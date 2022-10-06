#ifndef TOOLS_H
#define TOOLS_H

#include <random>
#include <signal.h>
#include <QString>
#include <map>
#include <QTimer>
#include "Src/DefinesLand.h"
#include "Src/Widgets/PopupWidget.h"


class QTouchEventFilter: public QObject
{
    Q_OBJECT
public:
    QTouchEventFilter(QObject *parent = 0);
    virtual ~QTouchEventFilter();
protected:
    bool eventFilter(QObject * p_obj, QEvent * p_event);
};

class QWidget;
class QFont;
class Tools
{
public :
    friend int main(int argc, char** argv); // ugly but I don't care for now at least constructor will be somewhat hidden

    static void RegisterHandlers();
    [[noreturn]] static void Handler(int sig);

    //======================================================================
    static void ParseKoboEreaderConf();
    bool IsLocalTimeFormatUS() const;
    const std::string GetFirmwareStr() const;
    void DisplayPopup(QString message, bool fullscreen = false, bool validateButton = true);
    PopupWidget* GetPopupInstance();
    DeviceState GetDeviceState() const;
    bool IsThereEnough(QcmExerciceType qcmType, int vocabPoolSize = -1) const;

    //======================================================================
    void RequestSleep();
    void RequestWakeUp();
    QTouchEventFilter* touchEventFilter = nullptr;

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

    int GetRandomInt(int rangeStart = 0, int rangeEnd = 100);
    std::mt19937& MT() { return mt; }
    std::default_random_engine& Rng_Engine() { return rng_engine; }

    static bool CorrectFontSize(QString const& text, QFont const& inFont, QWidget const& widget, QFont& correctedFnt);

private :
    Tools();
    ~Tools();

    //======================================================================
    void InstallGlobalEventFilter(bool enable);
    static void Sleep();
    static void WakeUp();
//    bool IsSleepAuthorized();

    //======================================================================
    std::random_device rd_device;
    std::mt19937 mt;
    std::default_random_engine rng_engine;

    std::map<int, std::string> handledErrors = { {SIGINT, "SIGINT"}, {SIGALRM, "SIGALRM"}, {SIGSEGV, "SIGSEGV"}, {SIGILL, "SIGILL"},
                                                 {SIGFPE, "SIGFPE"}, {SIGABRT, "SIGABRT"}, {SIGBUS, "SIGBUS"}, {SIGUSR1, "SIGUSR1"},
                                                 {SIGUSR2, "SIGUSR2"}, {SIGSYS, "SIGSYS"}};
    bool isLocalTimeFormatUS;
    std::string firmwareStr;
    PopupWidget* popup;
    QTimer wakeUpTimer;
    QTimer sleepTimer;

    static DeviceState deviceState;
    static bool sleepError;
};

#endif // TOOLS_H
