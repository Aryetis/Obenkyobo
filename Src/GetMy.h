#ifndef GETMY_H
#define GETMY_H
#include <QSettings>
#include "koboplatformfunctions.h"

class QcmExercice;
class FntSetting;
class AppSettings;
class SymbolSetting;
class ScreenSettings;
class MainWindow;
class VocabularyLearnEditSet;
class VocabularyDisplay;
class GetMy
{
public:
    static GetMy & Instance()
    {
        static GetMy instance;
        return instance;
    }

    void SetMainWindowWidget(MainWindow* mainWindow) { mainWindowInstance = mainWindow;}
    MainWindow& MainWindowWidget() { return *mainWindowInstance; }

    void SetQcmExerciceWidget(QcmExercice* exercice) { qcmExerciceInstance = exercice;}
    QcmExercice& QcmExerciceWidget() { return *qcmExerciceInstance; }

    void SetFntSettingWidget(FntSetting* fntSetting) { fntSettingInstance = fntSetting;}
    FntSetting& FntSettingWidget() { return *fntSettingInstance; }

    void SetAppSettingWidget(AppSettings* appSetting) { appSettingsInstance = appSetting;}
    AppSettings& AppSettingWidget() { return *appSettingsInstance; }

    void SetSymbolSettingWidget(SymbolSetting* symbolSetting) { symbolSettingInstance = symbolSetting;}
    SymbolSetting& SymbolSettingWidget() { return *symbolSettingInstance; }

    void SetScreenSettingsWidget(ScreenSettings* screenSettings) { screenSettingsInstance = screenSettings; }
    ScreenSettings& ScreenSettingsWidget() { return *screenSettingsInstance; }

    void SetVocabularyLearnEditSetWidget(VocabularyLearnEditSet* vocab) { vocabularyLearnEditSetInstance = vocab;}
    VocabularyLearnEditSet* VocabularyLearnEditSetWidget() { return vocabularyLearnEditSetInstance; }

    void SetVocabularyDisplayWidget(VocabularyDisplay* vocab) { vocabularyDisplayInstance = vocab;}
    VocabularyDisplay* VocabularyDisplayWidget() { return vocabularyDisplayInstance; }

    void SetSettingSerialier(QSettings* settingSerializer) { settingSerializerInstance = settingSerializer;}
    QSettings* SettingSerializer() { return settingSerializerInstance; }

    const KoboDeviceDescriptor& Descriptor() { return desc; }
    void ClearScreen() const
    {
        KoboPlatformFunctions::setFullScreenRefreshMode(WaveForm::WaveForm_GC16); // overkill
        KoboPlatformFunctions::clearScreen(true);
    }

private :
    GetMy() = default;
    MainWindow*                     mainWindowInstance;
    QcmExercice*                    qcmExerciceInstance;
    FntSetting*                     fntSettingInstance;
    AppSettings*                    appSettingsInstance;
    ScreenSettings*                 screenSettingsInstance;
    SymbolSetting*                  symbolSettingInstance;
    VocabularyLearnEditSet*         vocabularyLearnEditSetInstance;
    VocabularyDisplay*              vocabularyDisplayInstance;

    KoboDeviceDescriptor desc =  KoboPlatformFunctions::getKoboDeviceDescriptor();

    QSettings* settingSerializerInstance;
};


#endif // GETMY_H
