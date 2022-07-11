#ifndef GETMY_H
#define GETMY_H

#include <QSettings>
#include "koboplatformfunctions.h"

class QcmExercicePage;
class FntSettingsPage;
class AppSettingsPage;
class KanaEditPage;
class ScreenSettingsPage;
class MainWindow;
class VocabExplorerPage;
class VocabularyDisplayPage;
class GetMy
{
public:
    static GetMy & Instance()
    {
        static GetMy instance;
        return instance;
    }

    void SetMainWindowInst(MainWindow* mainWindow) { mainWindowInstance = mainWindow;}
    MainWindow& MainWindowInst() { return *mainWindowInstance; }

    void SetQcmExercicePageInst(QcmExercicePage* exercice) { qcmExerciceInstance = exercice;}
    QcmExercicePage& QcmExercicePageInst() { return *qcmExerciceInstance; }

    void SetFntSettingsPageInst(FntSettingsPage* fntSetting) { fntSettingInstance = fntSetting;}
    FntSettingsPage& FntSettingsPageInst() { return *fntSettingInstance; }

    void SetAppSettingsPageInst(AppSettingsPage* appSetting) { appSettingsInstance = appSetting;}
    AppSettingsPage& AppSettingsPageInst() { return *appSettingsInstance; }

    void SetKanaEditPageInst(KanaEditPage* symbolSetting) { KanaEditInstance = symbolSetting;}
    KanaEditPage& KanaEditPageInst() { return *KanaEditInstance; }

    void SetScreenSettingsPageInst(ScreenSettingsPage* screenSettings) { screenSettingsInstance = screenSettings; }
    ScreenSettingsPage& ScreenSettingsPageInst() { return *screenSettingsInstance; }

    void SetVocabExplorerPageInst(VocabExplorerPage* vocab) { vocabularyLearnEditSetInstance = vocab;}
    VocabExplorerPage* VocabExplorerPageInst() { return vocabularyLearnEditSetInstance; }

    void SetVocabularyDisplayPageInst(VocabularyDisplayPage* vocab) { vocabularyDisplayInstance = vocab;}
    VocabularyDisplayPage* VocabularyDisplayPageInst() { return vocabularyDisplayInstance; }

    void SetSettingSerialierInst(QSettings* settingSerializer) { settingSerializerInstance = settingSerializer;}
    QSettings* SettingSerializerInst() { return settingSerializerInstance; }

    const KoboDeviceDescriptor& Descriptor() { return desc; }

private :
    GetMy() = default;
    MainWindow*                     mainWindowInstance;
    QcmExercicePage*                qcmExerciceInstance;
    FntSettingsPage*                fntSettingInstance;
    AppSettingsPage*                appSettingsInstance;
    ScreenSettingsPage*             screenSettingsInstance;
    KanaEditPage*                   KanaEditInstance;
    VocabExplorerPage*         vocabularyLearnEditSetInstance;
    VocabularyDisplayPage*              vocabularyDisplayInstance;

    KoboDeviceDescriptor desc =  KoboPlatformFunctions::getKoboDeviceDescriptor();

    QSettings* settingSerializerInstance;
};


#endif // GETMY_H
