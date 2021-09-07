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
        luminosity = settingsSerializer->value("ScreenSettings/luminosity", desc.frontlightSettings.frontlightMax / 2).toInt();
        ui->LuminositySlider->setValue(luminosity);
        ui->LuminositySlider->setMinimum(desc.frontlightSettings.frontlightMin);
        ui->LuminositySlider->setMaximum(desc.frontlightSettings.frontlightMax);
        noSettingAvailable = false;
    }
    else
        ui->LuminositySlider->setVisible(false);

    if (desc.frontlightSettings.hasNaturalLight)
    {
        tint = settingsSerializer->value("ScreenSettings/tint", desc.frontlightSettings.naturalLightMax / 2).toInt();
        ui->TintSlider->setValue(tint);
        ui->TintSlider->setMinimum(desc.frontlightSettings.naturalLightMin);
        ui->TintSlider->setMaximum(desc.frontlightSettings.naturalLightMax);
        noSettingAvailable = false;
    }
    else
        ui->TintSlider->setVisible(false);

    if (!noSettingAvailable)
    {
        ui->NoSettingsAvailable->setVisible(false);
        KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
    }

    GetMy::GetInstance().SetScreenSettingsWidget(this);
}

ScreenSettings::~ScreenSettings()
{
    delete ui;
}

void ScreenSettings::on_LuminositySlider_valueChanged(int value)
{
    luminosity = value;
    settingsSerializer->setValue("ScreenSettings/luminosity", value);
    KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}

void ScreenSettings::on_TintSlider_valueChanged(int value)
{
    tint = value;
    settingsSerializer->setValue("ScreenSettings/tint", value);
    KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}
