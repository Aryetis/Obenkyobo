#include <unistd.h>
#include <fcntl.h>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QLabel>
#include <QProcess>
#include <iostream>
#include <execinfo.h>
#include "Src/Tools.h"
#include "Src/GetMy.h"
#include "Src/Pages/ScreenSettingsPage.h"
#include "Src/mainwindow.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/Widgets/PopupWidget.h"
#include "Src/KanasTables.h"

void Tools::RegisterHandlers()
{
    for(const auto& sig : GetMy::Instance().ToolsInst()->handledErrors)
        signal(sig.first, Handler);
}

void Tools::Handler(int sig)
{
    void *array[32];
    int size = backtrace(array, 32);

    std::cerr << "==========================================================" << std::endl;
    std::cerr << "ERROR: signal " << GetMy::Instance().ToolsInst()->handledErrors[sig] << ":" << std::endl;
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    std::cerr << "==========================================================" << std::endl;
    exit(sig);
}

//======================================================================

void Tools::ParseKoboEreaderConf()
{
    QFile inputFile("/mnt/onboard/.kobo/Kobo/Kobo eReader.conf");
    inputFile.open(QIODevice::ReadOnly);
    if (!inputFile.isOpen())
    {
        std::cerr << "ERROR: Couldn't open Kobo eReader.conf" << std::endl;
        return;
    }

    QTextStream stream(&inputFile);
    for (QString line = stream.readLine(); !line.isNull(); line = stream.readLine())
    {
        QStringList parsed = line.split("=", Qt::SkipEmptyParts);
        if (parsed.size() == 2)
        {
            QString prefix = parsed[0];
            QString value = parsed[1];
            if (prefix.compare("CurrentLocale") == 0)
            {
                if (value.size()>=2)
                {
                    value = parsed[1].mid(0,2);
                    GetMy::Instance().ToolsInst()->isLocalTimeFormatUS = (value.compare("en") == 0) ? true : false;
                }
                else
                    GetMy::Instance().ToolsInst()->isLocalTimeFormatUS = false;
            }
            else if(prefix.compare("EarliestChangeLog") == 0)
            {
                GetMy::Instance().ToolsInst()->firmwareStr = parsed[1].toStdString();
            }
        }
    }
}

bool Tools::IsLocalTimeFormatUS() const
{
    return isLocalTimeFormatUS;
}

const std::string Tools::GetFirmwareStr() const
{
    return firmwareStr;
}

void Tools::DisplayPopup(QString message, bool fullscreen)
{
    PopupWidget pop(message, fullscreen);
    pop.exec();
}

DeviceState Tools::GetDeviceState() const { return deviceState; }

bool Tools::IsThereEnough(QcmExerciceType qcmType, int vocabPoolSize /*= 0*/) const
{
    int minRequiredSymbol = GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryLine() *
            GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryRow();

    switch (qcmType)
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        {
            return (KanasTables::HiraganaSymbolsTableFamily.NbrOfEnabled() >= minRequiredSymbol);
        }
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            return (KanasTables::KatakanaSymbolsTableFamily.NbrOfEnabled() >= minRequiredSymbol);
        }
        case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        {
            if (vocabPoolSize == -1)
                std::cout << "ERROR: Tools::IsThereEnough() testing Vocabulary pool without any PoolSize parameter" << std::endl;
            return (vocabPoolSize >= minRequiredSymbol);
        }
    }

    assert(false);
}

//======================================================================

void Tools::Sleep() // needs to turn off wifi, stop printing stuff on screen (like clock, battery level, etc), etc
{
    if (deviceState != DeviceState::awake)
        return;

    deviceState = DeviceState::busy;

    qApp->processEvents();

    std::cout << "LOG: going to sleep" << std::endl;
    GetMy::Instance().ScreenSettingsPageInst().OnSleep(); // TODO : replace with signals at some point
    GetMy::Instance().MainWindowInst().OnSleep();

    std::cout << "LOG: disabling WiFi" << std::endl;
    KoboPlatformFunctions::disableWiFiConnection();

    //-------------------------------------------------------------
    std::cout << "LOG: /sys/power/stateExtendedFile << 1 (1st stage)" << std::endl;
    QFile stateExtendedFile("/sys/power/state-extended");
    if (!stateExtendedFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cout << "ERROR: Couldn't open /sys/power/state-extended (1st stage)" << std::endl;
        return;
    }
    else
    {
        QTextStream out(&stateExtendedFile);
        out << "1\n";
    }
    stateExtendedFile.close();

    //-------------------------------------------------------------
    std::cout << "LOG: sync" << std::endl;
    QThread::sleep(2);
    QProcess::execute("sync", {});

    //-------------------------------------------------------------
    QFile stateFile("/sys/power/state");
    if (!stateFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cout << "ERROR: Couldn't open /sys/power/state" << std::endl;

        QFile stateExtendedFile2("/sys/power/state-extended");
        if (!stateExtendedFile2.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            std::cout << "ERROR: Couldn't open /sys/power/state-extended (2nd stage)" << std::endl;
            return;
        }
        else
        {
            std::cout << "LOG: /sys/power/state-extended << 0 (2nd stage)" << std::endl;
            QTextStream out(&stateExtendedFile2);
            out << "0\n";
        }
        stateExtendedFile2.close();
    }
    else
    {
        std::cout << "LOG: /sys/power/state << mem (2nd stage)" << std::endl;
        QTextStream out(&stateFile);
        out << "mem\n";
    }
    stateFile.close();

    //-------------------------------------------------------------

    std::cout << "sleeping" << std::endl;
    deviceState = DeviceState::asleep; // probably useless...
}

void Tools::WakeUp()
{
    if (deviceState != DeviceState::asleep)
        return;

    deviceState = DeviceState::busy;
    std::cout << "LOG: Waking up" << std::endl;

    GetMy::Instance().ScreenSettingsPageInst().OnWakeUp();  // TODO : replace with signals at some point
    GetMy::Instance().MainWindowInst().OnWakeUp();

    //-------------------------------------------------------------
    QFile file("/sys/power/state-extended");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cout << "ERROR: Couldn't open /sys/power/state-extended" << std::endl;
        return;
    }
    else
    {
        QTextStream out(&file);
        out << "2\n";
    }
    file.close();

    //-------------------------------------------------------------
    QThread::msleep(100);

    //-------------------------------------------------------------
    file.setFileName("/sys/devices/virtual/input/input1/neocmd");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        std::cout << "LOG: Couldn't open /sys/devices/virtual/input/input1/neocmd" << std::endl;
    else
    {
        QTextStream out(&file);
        out << "2\n";
    }
    file.close();

    //-------------------------------------------------------------
//    GetMy::Instance().ScreenSettingsPageInst().OnWakeUp();  // TODO : replace with signals at some point
//    GetMy::Instance().MainWindowInst().OnWakeUp();

    if (GetMy::Instance().AppSettingsPageInst().GetWifiStatus())
        KoboPlatformFunctions::enableWiFiConnection();

    std::cout << "LOG: Woken up, ready to go" << std::endl;
    deviceState = DeviceState::awake;
}

//======================================================================

int Tools::GetRandomInt(int rangeStart, int rangeEnd)
{
    std::uniform_int_distribution<> distr(rangeStart, rangeEnd);
    return distr(rng_engine);
}

//======================================================================
bool Tools::CorrectFontSize(QString const& text, QFont const& inFont, QWidget const& widget, QFont& correctedFnt)
{
    correctedFnt = QFont(inFont);
    if (text=="")
        return false;

    QRectF newFontSizeRect;
    float curSize = correctedFnt.pointSizeF();
    float step = curSize/2.0;

    if (step<=CORRECTED_FONT_PRECISION)
        step = CORRECTED_FONT_PRECISION*5.0;

    float lastTestedSize = curSize;
    float curH = 0, curW = 0;

    std::cout << "[CorrectFontSize] widget.rect().width() : " << widget.rect().width() <<  " widget.rect().height() : " << widget.rect().height() << std::endl;

    bool firstLoop = true;
    while(step>CORRECTED_FONT_PRECISION || (curH > widget.rect().height()) || (curW > widget.rect().width()))
    {
        lastTestedSize = curSize;

        correctedFnt.setPointSizeF(curSize);
        QFontMetricsF fm{correctedFnt};

        QLabel const* label = qobject_cast<QLabel const*>(&widget);
        newFontSizeRect = fm.boundingRect(widget.rect(), label != nullptr
                                            ? (label->wordWrap() ? Qt::TextWordWrap : 0) | label->alignment() : 0,
                                          text);
        curH = newFontSizeRect.height();
        curW = newFontSizeRect.width();

        if ((curH > widget.rect().height()) || (curW > widget.rect().width()))
        {
            curSize -=step;
            if (step>CORRECTED_FONT_PRECISION)
                step/=2.0;
            if (curSize<=CORRECTED_MINIMAL_FONT_SIZE)
                break;
        }
        else
        {
            if (firstLoop) // if "first loop" && text is "too small"
                return false; // don't want to increase the size if text is smaller, only decrease if bigger
            else
                curSize +=step;
        }
        firstLoop = false;
    }

    correctedFnt.setPointSizeF(std::floor(lastTestedSize));
    return correctedFnt.pointSizeF() != std::floor(inFont.pointSizeF());
}


/******************************** private ********************************/
Tools::Tools()
{
    mt = std::mt19937(rd_device());
    rng_engine = std::default_random_engine{};
    deviceState = DeviceState::awake;
    isLocalTimeFormatUS = false;
    firmwareStr = "";
}
