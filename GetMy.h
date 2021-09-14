#ifndef GETMY_H
#define GETMY_H
#include <QSettings>
#include "koboplatformfunctions.h"

class QcmExercice;
class FntSetting;
class AppSettings;
class SymbolSetting;
class ScreenSettings;
class GetMy
{
public:
    static GetMy & GetInstance()
    {
        static GetMy instance;
        return instance;
    }

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

    void SetSettingSerialier(QSettings* settingSerializer) { settingSerializerInstance = settingSerializer;}
    QSettings* SettingSerializer() { return settingSerializerInstance; }

    const KoboDeviceDescriptor& Descriptor() { return desc; }

private :
    GetMy() = default;
    QcmExercice* qcmExerciceInstance;
    FntSetting* fntSettingInstance;
    AppSettings* appSettingsInstance;
    ScreenSettings* screenSettingsInstance;
    SymbolSetting* symbolSettingInstance;

    KoboDeviceDescriptor desc =  KoboPlatformFunctions::getKoboDeviceDescriptor();

    QSettings* settingSerializerInstance;
};


#endif // GETMY_H
