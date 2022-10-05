#include <unistd.h>
#include <fcntl.h>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QLabel>
#include <QProcess>
#include <iostream>
#include <execinfo.h>
#include <QApplication>
#include <QDateTime>
#include <QKeyEvent>
#include "Src/Tools.h"
#include "Src/GetMy.h"
#include "Src/Pages/ScreenSettingsPage.h"
#include "Src/mainwindow.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/KanasTables.h"


//======================================================================
//======================================================================
//======================================================================
void Tools::RegisterHandlers()
{
    for(const auto& sig : GetMy::Instance().ToolsInst()->handledErrors)
        signal(sig.first, Handler);
}

//======================================================================
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
        std::cerr << "ERROR: Couldn't open /mnt/onboard/.kobo/Kobo/Kobo eReader.conf" << std::endl;
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
        }
    }
    inputFile.close();

    inputFile.setFileName("/mnt/onboard/.kobo/version");
    // content : N87309B115301,4.1.15,4.33.19759,4.1.15,4.1.15,00000000-0000-0000-0000-000000000384
    // [model], [qt-version?], [firmware], [qt-version?], [qt-version?], [?]
    inputFile.open(QIODevice::ReadOnly);
    if (!inputFile.isOpen())
    {
        std::cerr << "ERROR: Couldn't open /mnt/onboard/.kobo/version" << std::endl;
        return;
    }
    stream.setDevice(&inputFile);
    QString fileContent = stream.readAll();
    QStringList elements = fileContent.split(',');
    if (elements.size() >= 3)
        GetMy::Instance().ToolsInst()->firmwareStr = elements[2].toStdString();
    inputFile.close();
}

//======================================================================
bool Tools::IsLocalTimeFormatUS() const
{
    return isLocalTimeFormatUS;
}

//======================================================================
const std::string Tools::GetFirmwareStr() const
{
    return firmwareStr;
}

void Tools::DisplayPopup(QString message, bool fullscreen /*= false*/, bool validateButton /*= true*/)
{
    popup = new PopupWidget(message, fullscreen, validateButton);
    if (validateButton)
        popup->exec();
    else
    {
        popup->show();
        QApplication::processEvents();
    }
}

//======================================================================
PopupWidget *Tools::GetPopupInstance()
{
    return popup;
}

//======================================================================
DeviceState Tools::GetDeviceState() const { return deviceState; }

//======================================================================
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
                std::cerr << "ERROR: Tools::IsThereEnough() testing Vocabulary pool without any PoolSize parameter" << std::endl;
            return (vocabPoolSize >= minRequiredSymbol);
        }
    }

    assert(false);
}

//======================================================================
void Tools::Sleep() // needs to turn off wifi, stop printing stuff on screen (like clock, battery level, etc), etc
{
    std::cout << "LOG: Sleep requested" << std::endl;

    if (deviceState != DeviceState::awake || QDateTime::currentSecsSinceEpoch() < lastWakeUpDateInS + HACKY_SLEEP_COOLDOWN)
    {
        std::cout << "LOG: Sleep request denied" << std::endl;
        return;
    }

    std::cout << "!!! DEVICE STATE = BUSY" << std::endl;
    deviceState = DeviceState::busy;

    // Fake Sleep for old devices while charging
//    if (!IsSleepAuthorized())
//    {
//        GetMy::Instance().ToolsInst()->DisplayPopup("Sorry, your device does not support \"Sleep and Charge\" functionality. Turning off light and wifi for you");

//        std::cout << "LOG: Faking sleep" << std::endl;
//        KoboPlatformFunctions::disableWiFiConnection();
//        GetMy::Instance().ScreenSettingsPageInst().OnSleep();
//        GetMy::Instance().MainWindowInst().OnSleep();
//        deviceState = DeviceState::asleep; // to indicate to WakeUp() that slept went ""well""
//        return;
//    }

    IgnoreAllInputs(true);
    std::cout << "LOG: going to sleep" << std::endl;

    GetMy::Instance().ToolsInst()->DisplayPopup("Sleeping", true, false);
    qApp->processEvents();

    GetMy::Instance().ScreenSettingsPageInst().OnSleep();
    GetMy::Instance().MainWindowInst().OnSleep();

    std::cout << "LOG: disabling WiFi" << std::endl;
    KoboPlatformFunctions::disableWiFiConnection(); // MANDATORY !!!!!

    //-------------------------------------------------------------
    std::cout << "LOG: /sys/power/stateExtendedFile << 1 (1st stage)" << std::endl;
    QFile stateExtendedFile("/sys/power/state-extended");
    if (!stateExtendedFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cerr << "ERROR: Couldn't open /sys/power/state-extended (1st stage)" << std::endl;
        sleepError = true;
    }
    else
    {
        QTextStream out(&stateExtendedFile);
        out << "1\n";
        if (out.status() != QTextStream::Ok)
        {
            std::cerr << "ERROR: Couldn't write to /sys/power/state-extended (1st stage)" << std::endl;
            sleepError = true;
        }
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
        std::cerr << "ERROR: Couldn't open /sys/power/state" << std::endl;

        QFile stateExtendedFile2("/sys/power/state-extended");
        if (!stateExtendedFile2.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            std::cerr << "ERROR: Couldn't open /sys/power/state-extended (2nd stage)" << std::endl;
            GetMy::Instance().ToolsInst()->DisplayPopup("ERROR : couldn't go to sleep, please report the error and provide log.txt");
            sleepError = true;
        }
        else
        {
            std::cout << "LOG: /sys/power/state-extended << 0 (2nd stage)" << std::endl;
            QTextStream out(&stateExtendedFile2);
            out << "0\n";
            if (out.status() != QTextStream::Ok)
            {
                std::cerr << "ERROR: Couldn't write to /sys/power/state-extended (2nd stage)" << std::endl;
                sleepError = true;
            }
        }
        stateExtendedFile2.close();
    }
    else
    {
        std::cout << "LOG: /sys/power/state << mem (2nd stage)" << std::endl;
        QTextStream out(&stateFile);
        out << "mem\n";
        if (out.status() != QTextStream::Ok)
        {
            std::cerr << "ERROR: Couldn't write to /sys/power/state << mem (2nd stage)" << std::endl;
            sleepError = true;
        }
    }
    stateFile.close();

    //-------------------------------------------------------------
    // Everything below here will be reached when waking up,
    // WakeUp is triggered by system upon next PowerButton press regardless of Obenkyobo
    //-------------------------------------------------------------
    std::cout << "!!! DEVICE STATE = ASLEEP" << std::endl;
    deviceState = DeviceState::asleep; // to indicate to WakeUp() that slept went well
//    IgnoreAllInputs(false); // so the Press can be detected to trigger WakeUp() function
    WakeUp();
}

//======================================================================
void Tools::WakeUp()
{
    std::cout << "LOG: Wake Up requested" << std::endl;

    if (deviceState != DeviceState::asleep)
        return;

//    IgnoreAllInputs(true); // ignore everything during busy process
    std::cout << "LOG: Waking up" << std::endl;

    GetMy::Instance().ToolsInst()->GetPopupInstance()->accept();
    GetMy::Instance().ToolsInst()->DisplayPopup("Waking Up", true, false);
    std::cout << "!!! DEVICE STATE = BUSY" << std::endl;
    deviceState = DeviceState::busy;

    QThread::sleep(2);

    GetMy::Instance().ScreenSettingsPageInst().OnWakeUp();  // TODO : replace with signals at some point
    GetMy::Instance().MainWindowInst().OnWakeUp();

    //-------------------------------------------------------------
    QFile file("/sys/power/state-extended");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cerr << "ERROR: Couldn't open /sys/power/state-extended" << std::endl;
        sleepError = true;
    }
    else
    {
        QTextStream out(&file);
        out << "2\n";
        if (out.status() != QTextStream::Ok)
        {
            std::cerr << "ERROR: Couldn't write to /sys/power/state-extended" << std::endl;
            sleepError = true;
        }
    }
    file.close();

    //-------------------------------------------------------------
    QThread::msleep(100);

    //-------------------------------------------------------------
    file.setFileName("/sys/devices/virtual/input/input1/neocmd");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        std::cout << "LOG: Couldn't open /sys/devices/virtual/input/input1/neocmd (not necessarly an error)" << std::endl;
    else
    {
        QTextStream out(&file);
        out << "2\n";
        if (out.status() != QTextStream::Ok)
        {
            std::cerr << "ERROR: Couldn't write to /sys/devices/virtual/input/input1/neocmd" << std::endl;
            sleepError = true;
        }
    }
    file.close();

    //-------------------------------------------------------------
    if (GetMy::Instance().AppSettingsPageInst().GetWifiStatus())
        KoboPlatformFunctions::enableWiFiConnection();

    qApp->processEvents();
    IgnoreAllInputs(false);

    //-------------------------------------------------------------
    if (sleepError)
    {
        GetMy::Instance().ToolsInst()->DisplayPopup("ERROR : Couldn't sleep/wake up properly, please report the error on Obenkyobo's github page and provide log.txt if possible");
        return;
    }

    //-------------------------------------------------------------
    std::cout << "LOG: Woken up, ready to go" << std::endl;
    sleepError = false;
    GetMy::Instance().ToolsInst()->GetPopupInstance()->close();
    std::cout << "!!! DEVICE STATE = AWAKE" << std::endl;
    lastWakeUpDateInS = QDateTime::currentSecsSinceEpoch();
    deviceState = DeviceState::awake;
}

bool Tools::IsSleepAuthorized()
{
    QList<KoboDevice> NoChargeSleepList
    {
        KoboDevice::KoboTouchAB,
        KoboDevice::KoboTouchC,
        KoboDevice::KoboMini,
        KoboDevice::KoboGlo,
        KoboDevice::KoboAura,
        KoboDevice::KoboAuraHD,
        KoboDevice::KoboAuraH2O
    }; // Informed guess ... https://discord.com/channels/809205711778480158/958419944243089479/999974500756103188
       // to be confirmed as my glo can go to sleep and charge ?

    return !(KoboPlatformFunctions::isBatteryCharging() && NoChargeSleepList.contains(GetMy::Instance().Descriptor().device));
}

//======================================================================
void Tools::IgnoreAllInputs(bool enable)
{
    std::cout << "LOG: IgnoreAllInputs(" << (enable?"true)":"false)") << std::endl;
    if (enable)
    {
        if (touchEventFilter == nullptr)
            touchEventFilter =  new QTouchEventFilter();
        QApplication::instance()->installEventFilter(touchEventFilter);
    }
    else if (touchEventFilter != nullptr)
        QApplication::instance()->removeEventFilter(touchEventFilter);
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

    bool firstLoop = true;
    while(step>CORRECTED_FONT_PRECISION || (curH > widget.rect().height()) || (curW > widget.rect().width()))
    {
        lastTestedSize = curSize;

        correctedFnt.setPointSizeF(curSize);
        QFontMetricsF fm{correctedFnt};

        QLabel const* label = qobject_cast<QLabel const*>(&widget);
        newFontSizeRect = fm.boundingRect(widget.rect(), label != nullptr
                                ? (label->wordWrap() ? Qt::TextWordWrap : 0) | label->alignment() : 0, text);
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
    popup = nullptr;
    lastWakeUpDateInS = QDateTime::currentSecsSinceEpoch();
}

//======================================================================
Tools::~Tools()
{
    if (popup != nullptr)
        delete popup;
    if (touchEventFilter != nullptr)
        delete touchEventFilter;
}

//======================================================================
//======================================================================
//======================================================================
QTouchEventFilter::QTouchEventFilter(QObject *parent) : QObject(parent) {}

//======================================================================
QTouchEventFilter::~QTouchEventFilter() {}

//======================================================================
bool QTouchEventFilter::eventFilter(QObject */*p_obj*/, QEvent *p_event)
{
    if (p_event->type() == QEvent::TouchBegin ||
            p_event->type() == QEvent::TouchUpdate ||
            p_event->type() == QEvent::TouchEnd ||
            p_event->type() == QEvent::TouchCancel ||
            p_event->type() == QEvent::MouseButtonPress ||
            p_event->type() == QEvent::MouseButtonRelease ||
            p_event->type() == QEvent::MouseButtonDblClick ||
            p_event->type() == QEvent::MouseMove ||                 // ignore standard touch mouse input
            (
                p_event->type() == QEvent::KeyPress &&
                (static_cast<QKeyEvent*>(p_event)->key() == KoboKey::Key_Power ||
                static_cast<QKeyEvent*>(p_event)->key() == KoboKey::Key_SleepCover) &&
                GetMy::Instance().ToolsInst()->GetDeviceState() == DeviceState::busy
            ) ||
            (
                p_event->type() == QEvent::KeyRelease &&
                (static_cast<QKeyEvent*>(p_event)->key() == KoboKey::Key_Power ||
                static_cast<QKeyEvent*>(p_event)->key() == KoboKey::Key_SleepCover) &&
                GetMy::Instance().ToolsInst()->GetDeviceState() == DeviceState::busy
            )
        )
    {
        std::cout << "!!! Event filtered during Busy state" << std::endl;
        p_event->accept(); // I acept it, I take care of it. Don't propagate it
        return true;
    }
    else if (p_event->type() == QEvent::KeyPress)
{
       std::cout << "!!! unfiltered KeyPress : " << static_cast<QKeyEvent*>(p_event)->key() << " while (asleep,awake,busy) :" << GetMy::Instance().ToolsInst()->GetDeviceState() << std::endl;
       return false;
}
    else
        return false;
}
