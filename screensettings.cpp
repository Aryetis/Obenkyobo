#include "screensettings.h"
#include "ui_screensettings.h"
#include "GetMy.h"

// TODO : fix tint not taking float value... that's in the qt-platform-plugin tho

ScreenSettings::ScreenSettings(QWidget *parent) :
    QWidget(parent), ui(new Ui::ScreenSettings),
    luminosity(0), tint(0), settingAvailable(false),
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
        ui->LuminosityValue->setText(QString::number(static_cast<int>(luminosity)));
        settingAvailable = true;
    }
    else
        ui->LuminosityContainer->setEnabled(false);

    if (desc.frontlightSettings.hasNaturalLight)
    {
        tint = settingsSerializer->value("ScreenSettings/tint", desc.frontlightSettings.naturalLightMax / 2.0).toFloat();
        ui->TintSlider->setValue(static_cast<int>(tint));
        ui->TintValue->setText(QString::number(static_cast<double>(tint)));
        settingAvailable = true;
    }
    else
        ui->TintContainer->setEnabled(false);

    if (settingAvailable)
        KoboPlatformFunctions::setFrontlightLevel(static_cast<int>(luminosity), static_cast<int>(tint));

    GetMy::GetInstance().SetScreenSettingsWidget(this);
}

ScreenSettings::~ScreenSettings()
{
    delete ui;
}

bool ScreenSettings::AreSettingsAvailablePopup() const
{
    if(!settingAvailable)
    {
        GetMy::GetInstance().DisplayPopup("Sorry, no Screen Settings (luminosity, tint) available for your ereader");
        return false;
    }
    return true;
}

void ScreenSettings::on_LuminositySlider_valueChanged(int value)
{
    luminosity = ((static_cast<float>(desc.frontlightSettings.frontlightMax - desc.frontlightSettings.frontlightMin)
                   / static_cast<float>(ui->LuminositySlider->maximum()) * static_cast<float>(value)))
                + static_cast<float>(desc.frontlightSettings.frontlightMin);
    settingsSerializer->setValue("ScreenSettings/luminosity", value);
    ui->LuminosityValue->setText(QString::number(static_cast<int>(luminosity)));
    KoboPlatformFunctions::setFrontlightLevel(static_cast<int>(luminosity), static_cast<int>(tint));
}

void ScreenSettings::on_TintSlider_valueChanged(int value)
{
    tint =  ((static_cast<float>(desc.frontlightSettings.naturalLightMax - desc.frontlightSettings.naturalLightMin)
              / static_cast<float>(ui->TintSlider->maximum()) * static_cast<float>(value)))
           + static_cast<float>(desc.frontlightSettings.naturalLightMin);
    settingsSerializer->setValue("ScreenSettings/tint", value);
    ui->TintValue->setText(QString::number(static_cast<double>(tint)));
    KoboPlatformFunctions::setFrontlightLevel(static_cast<int>(luminosity), static_cast<int>(tint));
}
