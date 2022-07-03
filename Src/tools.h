#ifndef TOOLS_H
#define TOOLS_H
#include <random>
#include <iterator>
#include <QString>
#include <QMessageBox>

#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
#include <fcntl.h>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <iostream>
#include <QGridLayout>
#include "Src/GetMy.h"
#include "Src/screensettings.h"
#include "Src/mainwindow.h"
#include "Src/appsettings.h"

#include <QLabel>
#include <QRect>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include "popup.h"

class Tools
{
public :
    static Tools & GetInstance() // register to GetMy:: ... might as well embrace the jank completly
    {
        static Tools instance;
        return instance;
    }

    //======================================================================
    static void RegisterHandlers()
    {
        for(const auto& sig : Tools::GetInstance().handledErrors)
            signal(sig.first, Handler);
    }

    [[noreturn]] static void Handler(int sig)
    {
        void *array[32];
        int size = backtrace(array, 32);

        std::cerr << "==========================================================" << std::endl;
        std::cerr << "ERROR: signal " << Tools::GetInstance().handledErrors[sig] << ":" << std::endl;
        backtrace_symbols_fd(array, size, STDERR_FILENO);
        std::cerr << "==========================================================" << std::endl;
        exit(sig);
    }

    //======================================================================
    static void ParseKoboEreaderConf()
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
                        Tools::GetInstance().isLocalTimeFormatUS = (value.compare("en") == 0) ? true : false;
                    }
                    else
                        Tools::GetInstance().isLocalTimeFormatUS = false;
                }
                else if(prefix.compare("EarliestChangeLog") == 0)
                {
                    Tools::GetInstance().firmwareStr = parsed[1].toStdString();
                }
            }
        }
    }


    //======================================================================
    bool IsLocalTimeFormatUS() const
    {
        return isLocalTimeFormatUS;
    }

    const std::string GetFirmwareStr() const
    {
        return firmwareStr;
    }

    //======================================================================
    template<typename Iter, typename RandomGenerator>
    Iter GetRandom(Iter start, Iter end, RandomGenerator& gen)
    {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(gen));
        return start;
    }

    template<typename Iter>
    Iter GetRandom(Iter start, Iter end)
    {
        return GetRandom(start, end, rng_engine);
    }

    int GetRandomInt(int rangeStart = 0, int rangeEnd = 100)
    {
        std::uniform_int_distribution<> distr(rangeStart, rangeEnd);
        return distr(rng_engine);
    }


    std::mt19937& MT() { return mt; }
    std::default_random_engine& Rng_Engine() { return rng_engine; }

    //======================================================================
    void DisplayPopup(QString message, bool fullscreen = false)
    {
        Popup pop(message, fullscreen);
        pop.exec();
    }

    //======================================================================
    enum DeviceState { asleep, awake, busy };
    DeviceState GetDeviceState() const { return deviceState; }

    void Sleep() // needs to turn off wifi, stop printing stuff on screen (like clock, battery level, etc), etc
    {
        if (deviceState != DeviceState::awake)
            return;

        deviceState = DeviceState::busy;

        qApp->processEvents();

        std::cout << "LOG: going to sleep" << std::endl;
        GetMy::Instance().ScreenSettingsWidget().OnSleep(); // TODO : replace with signals at some point
        GetMy::Instance().MainWindowWidget().OnSleep();

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

    void WakeUp()
    {
        if (deviceState != DeviceState::asleep)
            return;

        deviceState = DeviceState::busy;
        std::cout << "LOG: Waking up" << std::endl;

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
        GetMy::Instance().ScreenSettingsWidget().OnWakeUp();  // TODO : replace with signals at some point
        GetMy::Instance().MainWindowWidget().OnWakeUp();

        if (GetMy::Instance().AppSettingWidget().GetWifiStatus())
            KoboPlatformFunctions::enableWiFiConnection();

        std::cout << "LOG: Woken up, ready to go" << std::endl;
        deviceState = DeviceState::awake;
    }

//==========================================================================
private :
    Tools()
    {
        mt = std::mt19937(rd_device());
        rng_engine = std::default_random_engine{};
        deviceState = DeviceState::awake;
        isLocalTimeFormatUS = false;
        firmwareStr = "";
    }

    std::random_device rd_device;
    std::mt19937 mt;
    std::default_random_engine rng_engine;

    std::map<int, std::string> handledErrors = { {SIGINT, "SIGINT"}, {SIGALRM, "SIGALRM"}, {SIGSEGV, "SIGSEGV"}, {SIGILL, "SIGILL"},
                                                 {SIGFPE, "SIGFPE"}, {SIGABRT, "SIGABRT"}, {SIGBUS, "SIGBUS"}, {SIGUSR1, "SIGUSR1"},
                                                 {SIGUSR2, "SIGUSR2"}, {SIGSYS, "SIGSYS"}};
    DeviceState deviceState;
    bool isLocalTimeFormatUS;
    std::string firmwareStr;
};

#endif // TOOLS_H
