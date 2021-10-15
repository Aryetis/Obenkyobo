#include "Src/screensettings.h"
#include "ui_screensettings.h"
#include "Src/GetMy.h"
#include "Src/tools.h"

// Note : the whole kobo family frontlight drivers only take integeter, so we're good setting sliders's step to 1
ScreenSettings::ScreenSettings(QWidget *parent) :
    QWidget(parent), ui(new Ui::ScreenSettings),
    luminosity(0), tint(0), settingAvailable(false),
    desc(GetMy::Instance().Descriptor()),
    settingsSerializer(GetMy::Instance().SettingSerializer())
{
    ui->setupUi(this);

    // Screen setting must be ready before switching widget (especially if first startup is in the dark)
    // => don't delay the initialization
    if (desc.frontlightSettings.hasFrontLight)
    {
        luminosity = settingsSerializer->value("ScreenSettings/luminosity", desc.frontlightSettings.frontlightMax / 2.0).toInt();
        ui->LuminositySlider->setValue(luminosity);
        ui->LuminositySlider->setMinimum(desc.frontlightSettings.frontlightMin);
        ui->LuminositySlider->setMaximum(desc.frontlightSettings.frontlightMax);
        ui->LuminositySlider->setPageStep((desc.frontlightSettings.frontlightMax-desc.frontlightSettings.frontlightMin)/10);
        ui->LuminosityValue->setText(QString::number(luminosity));
        settingAvailable = true;
    }
    else
        ui->LuminosityContainer->setEnabled(false);

    if (desc.frontlightSettings.hasNaturalLight)
    {
        tint = settingsSerializer->value("ScreenSettings/tint", desc.frontlightSettings.naturalLightMax / 2.0).toInt();
        ui->TintSlider->setValue(static_cast<int>(tint));
        ui->TintSlider->setMinimum(desc.frontlightSettings.naturalLightMin);
        ui->TintSlider->setMaximum(desc.frontlightSettings.naturalLightMax);
        ui->TintSlider->setPageStep((desc.frontlightSettings.naturalLightMax-desc.frontlightSettings.naturalLightMin)/10);
        ui->TintValue->setText(QString::number(tint));
        settingAvailable = true;
    }
    else
        ui->TintContainer->setEnabled(false);

    if (settingAvailable)
        KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);

    GetMy::Instance().SetScreenSettingsWidget(this);
}

ScreenSettings::~ScreenSettings()
{
    delete ui;
}

bool ScreenSettings::AreSettingsAvailablePopup() const
{
    if(!settingAvailable)
    {
        Tools::GetInstance().DisplayPopup("Sorry, no Screen Settings (luminosity, tint) available for your ereader");
        return false;
    }
    return true;
}

void ScreenSettings::OnSleep() const
{
    if(settingAvailable)
        KoboPlatformFunctions::setFrontlightLevel(desc.frontlightSettings.naturalLightMin, tint);
}

void ScreenSettings::OnWakeUp() const
{
    if(settingAvailable)
        KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}

void ScreenSettings::on_LuminositySlider_valueChanged(int value)
{
    luminosity = value;
    settingsSerializer->setValue("ScreenSettings/luminosity", value);
    ui->LuminosityValue->setText(QString::number(luminosity));
    KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}

void ScreenSettings::on_TintSlider_valueChanged(int value)
{
    tint = value;
    settingsSerializer->setValue("ScreenSettings/tint", value);
    ui->TintValue->setText(QString::number(tint));
    KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}
