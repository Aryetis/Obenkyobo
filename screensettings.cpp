#include "screensettings.h"
#include "ui_screensettings.h"
#include "GetMy.h"

// TODO : fix weird layout empty space

ScreenSettings::ScreenSettings(QWidget *parent) :
    QWidget(parent), ui(new Ui::ScreenSettings),
    luminosity(0), tint(0), noSettingAvailable(true),
    desc(GetMy::GetInstance().Descriptor()),
    settingsSerializer(GetMy::GetInstance().SettingSerializer())
{
    ui->setupUi(this);

    // Screen setting must be ready before switching widget (especially if first startup is in the dark)
    // => don't delay the initialization
    if (desc.frontlightSettings.hasFrontLight)
    {
        luminosity = settingsSerializer->value("ScreenSettings/luminosity", desc.frontlightSettings.frontlightMax / 2.0).toFloat();
        ui->LuminositySlider->setValue(static_cast<int>(luminosity));
        noSettingAvailable = false;
    }
    else
        ui->LuminositySlider->setVisible(false);

    if (desc.frontlightSettings.hasNaturalLight)
    {
        tint = settingsSerializer->value("ScreenSettings/tint", desc.frontlightSettings.naturalLightMax / 2.0).toFloat();
        ui->TintSlider->setValue(static_cast<int>(tint));
        noSettingAvailable = false;
    }
    else
        ui->TintSlider->setVisible(false);

    if (!noSettingAvailable)
    {
        ui->NoSettingsAvailable->setVisible(false);
        KoboPlatformFunctions::setFrontlightLevel(static_cast<int>(luminosity), static_cast<int>(tint));
    }

    GetMy::GetInstance().SetScreenSettingsWidget(this);
}

ScreenSettings::~ScreenSettings()
{
    delete ui;
}

void ScreenSettings::on_LuminositySlider_valueChanged(int value)
{
    luminosity = ((static_cast<float>(desc.frontlightSettings.frontlightMax - desc.frontlightSettings.frontlightMin)
                   / static_cast<float>(ui->LuminositySlider->maximum()) * static_cast<float>(value)))
                + static_cast<float>(desc.frontlightSettings.frontlightMin);
    settingsSerializer->setValue("ScreenSettings/luminosity", luminosity);
    KoboPlatformFunctions::setFrontlightLevel(static_cast<int>(luminosity), static_cast<int>(tint));
}

void ScreenSettings::on_TintSlider_valueChanged(int value)
{
    tint =  ((static_cast<float>(desc.frontlightSettings.naturalLightMax - desc.frontlightSettings.naturalLightMin)
              / static_cast<float>(ui->TintSlider->maximum()) * static_cast<float>(value)))
           + static_cast<float>(desc.frontlightSettings.naturalLightMin);
    settingsSerializer->setValue("ScreenSettings/tint", tint);
    KoboPlatformFunctions::setFrontlightLevel(static_cast<int>(luminosity), static_cast<int>(tint));
}
