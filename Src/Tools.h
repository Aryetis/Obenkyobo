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
    static void RequestSleep();
    static void RequestWakeUp();
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

    static void BumpInactivityTimer();

private :
    Tools();
    ~Tools();

    //======================================================================
    void InstallGlobalEventFilter(bool enable);
    static void PreSleep();
    static void Sleep();
    static void WakeUp();
    static void PostWakeUp();
    static bool IsScreenSaverNeeded();

    //======================================================================
    std::random_device rd_device;
    std::mt19937 mt;
    std::default_random_engine rng_engine;

    inline static std::map<int, std::string> handledErrors { {SIGINT, "SIGINT"}, {SIGALRM, "SIGALRM"}, {SIGSEGV, "SIGSEGV"}, {SIGILL, "SIGILL"},
                                                             {SIGFPE, "SIGFPE"}, {SIGABRT, "SIGABRT"}, {SIGBUS, "SIGBUS"}, {SIGUSR1, "SIGUSR1"},
                                                             {SIGUSR2, "SIGUSR2"}, {SIGSYS, "SIGSYS"}};
    inline static bool isLocalTimeFormatUS;
    inline static std::string firmwareStr;
    inline static PopupWidget* popup;
    inline static QTimer wakeUpTimer = QTimer();
    inline static QTimer preSleepTimer = QTimer();
    inline static QTimer inactivityTimer = QTimer();

    inline static QTimer sleepTimer = QTimer();
    inline static QTimer postWakeUpTimer = QTimer();
    inline static bool sleepReached = false;
    inline static DeviceState deviceState = DeviceState::awake;
    inline static bool sleepError = false;
};

#endif // TOOLS_H
