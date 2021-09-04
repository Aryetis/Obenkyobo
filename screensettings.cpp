#include "screensettings.h"
#include "ui_screensettings.h"

// TODO NOW initialize value from system file
//    ui->LuminositySlider->SetValue();
//    ui->TintSlider->setValue();

ScreenSettings::ScreenSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenSettings),
    luminosity(0),
    tint(0),
    noSettingAvailable(true)
{
    ui->setupUi(this);

    desc = KoboPlatformFunctions::getKoboDeviceDescriptor();
    if (desc.frontlightSettings.hasFrontLight)
    {
        luminosity = desc.frontlightSettings.frontlightMax / 2;
        ui->LuminositySlider->setMinimum(desc.frontlightSettings.frontlightMin);
        ui->LuminositySlider->setMaximum(desc.frontlightSettings.frontlightMax);
        noSettingAvailable = false;
    } // TODO hide those by default and have a "woops sorry no screen settings" in case / or unregister the widget
    else
        ui->LuminositySlider->setVisible(false);

    if (desc.frontlightSettings.hasNaturalLight)
    {
        tint = desc.frontlightSettings.naturalLightMax / 2;
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
}

ScreenSettings::~ScreenSettings()
{
    delete ui;
}

void ScreenSettings::on_LuminositySlider_valueChanged(int value)
{
    luminosity = value;
    KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}

void ScreenSettings::on_TintSlider_valueChanged(int value)
{
    tint = value;
    KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}
