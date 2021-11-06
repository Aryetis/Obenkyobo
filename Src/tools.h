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
#include "Src/GetMy.h"
#include "Src/screensettings.h"

class Tools
{
public :
    static Tools & GetInstance()
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
                    if (value.size()>3)
                    {
                        value = parsed[1].mid(0,3);
                        Tools::GetInstance().isLocalTimeFormatUS = (value.compare("en_") == 0) ? true : false;
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
    void DisplayPopup(QString message)
    {
        QMessageBox popup;
        popup.setText(message);
        popup.setStyleSheet("QLabel{min-width: 300px;}"
                            "QMessageBox { border: 3px solid black }");
        popup.exec();
    }

    //======================================================================
    void Sleeping(bool _b) { sleeping = _b; }
    bool Sleeping() const { return sleeping; }

    void Sleep()
    {
        if (sleeping)
            return;

        std::cout << "going to sleep" << std::endl;
        GetMy::Instance().ScreenSettingsWidget().OnSleep();

KoboPlatformFunctions::disableWiFiConnection();

//        //-------------------------------------------------------------
//        QFile stateExtendedFile("/sys/power/state-extended");
//        if (!stateExtendedFile.open(QIODevice::WriteOnly | QIODevice::Text))
//            return;
//        else
//        {
//            QTextStream out(&stateExtendedFile);
//            out << "1\n";
//        }
//        stateExtendedFile.close();

//        //-------------------------------------------------------------
//        QThread::sleep(20);
//        QProcess::execute("sync", {});

//        //-------------------------------------------------------------
//        QFile stateFile("/sys/power/state-extended");
//        if (!stateFile.open(QIODevice::WriteOnly | QIODevice::Text))
//        {
//            QFile stateExtendedFile2("/sys/power/state-extended");
//            if (!stateExtendedFile2.open(QIODevice::WriteOnly | QIODevice::Text))
//                return;
//            else
//            {
//                QTextStream out(&stateExtendedFile2);
//                out << "0\n";
//            }
//            stateExtendedFile2.close();
//        }
//        else
//        {
//            QTextStream out(&stateFile);
//            out << "mem\n";
//        }
//        stateFile.close();

//        //-------------------------------------------------------------
//        std::cout << "sleeping" << std::endl;
//        sleeping = true;
    }

    void WakeUp()
    {
        if (!sleeping)
            return;

        std::cout << "Waking up" << std::endl;
        GetMy::Instance().ScreenSettingsWidget().OnWakeUp();

KoboPlatformFunctions::enableWiFiConnection();

//        //-------------------------------------------------------------
//        QFile file("/sys/power/state-extended");
//        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//            return;
//        else
//        {
//            QTextStream out(&file);
//            out << "2\n";
//        }
//        file.close();

//        //-------------------------------------------------------------
//        QThread::msleep(100);

//        //-------------------------------------------------------------
//        file.setFileName("/sys/devices/virtual/input/input1/neocmd");
//        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//            return;
//        else
//        {
//            QTextStream out(&file);
//            out << "2\n";
//        }
//        file.close();

//        //-------------------------------------------------------------
//        std::cout << "Woken up, that's done" << std::endl;
//        sleeping = false;
    }

//==========================================================================
private :
    Tools()
    {
        mt = std::mt19937(rd_device());
        rng_engine = std::default_random_engine{};
        sleeping = false;
        isLocalTimeFormatUS = false;
        firmwareStr = "";
    }

    std::random_device rd_device;
    std::mt19937 mt;
    std::default_random_engine rng_engine;

    std::map<int, std::string> handledErrors = { {SIGINT, "SIGINT"}, {SIGALRM, "SIGALRM"}, {SIGSEGV, "SIGSEGV"}, {SIGILL, "SIGILL"},
                                                 {SIGFPE, "SIGFPE"}, {SIGABRT, "SIGABRT"}, {SIGBUS, "SIGBUS"}, {SIGUSR1, "SIGUSR1"},
                                                 {SIGUSR2, "SIGUSR2"}, {SIGSYS, "SIGSYS"}};
    bool sleeping;
    bool isLocalTimeFormatUS;
    std::string firmwareStr;
};

#endif // TOOLS_H
