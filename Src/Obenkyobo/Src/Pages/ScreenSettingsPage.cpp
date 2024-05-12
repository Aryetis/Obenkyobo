#include "Src/Pages/ScreenSettingsPage.h"
#include "ui_ScreenSettingsPage.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"

// Note : the whole kobo family frontlight drivers only take integeter, so we're good setting sliders's step to 1
ScreenSettingsPage::ScreenSettingsPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::ScreenSettingsPage),
    luminosity(0), tint(0), settingAvailable(false),
    lightToggleStatus(true),
    extraFunk(GetMy::Instance().ExtraFunk()),
    settingsSerializer(*GetMy::Instance().SettingSerializerInst())
{
    ui->setupUi(this);

    // Screen setting must be ready before switching widget (especially if first startup is in the dark)
    // => don't delay the initialization
    if (extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.hasFrontLight)
    {
        luminosity = settingsSerializer.value("ScreenSettings/luminosity", extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.frontlightMax / 2.0).toInt();
        ui->LuminositySlider->setValue(luminosity);
        ui->LuminositySlider->setMinimum(extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.frontlightMin);
        ui->LuminositySlider->setMaximum(extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.frontlightMax);
        ui->LuminositySlider->setPageStep((extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.frontlightMax-extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.frontlightMin)/10);
        ui->LuminosityValue->setText(QString::number(luminosity));
        settingAvailable = true;
    }
    else
    {
        ui->LuminosityLabel->setVisible(false);
        ui->LuminosityValue->setVisible(false);
        ui->LuminositySlider->setVisible(false);
    }

    if (extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.hasNaturalLight)
    {
        tint = settingsSerializer.value("ScreenSettings/tint", extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.naturalLightMax / 2.0).toInt();
        ui->TintSlider->setValue(static_cast<int>(tint));
        ui->TintSlider->setMinimum(extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.naturalLightMin);
        ui->TintSlider->setMaximum(extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.naturalLightMax);
        ui->TintSlider->setPageStep((extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.naturalLightMax-extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.naturalLightMin)/10);
        ui->TintValue->setText(QString::number(tint));
        settingAvailable = true;
    }
    else
    {
        ui->TintLabel->setVisible(false);
        ui->TintValue->setVisible(false);
        ui->TintSlider->setVisible(false);
    }

    if (settingAvailable)


    GetMy::Instance().SetScreenSettingsPageInst(this);
}

ScreenSettingsPage::~ScreenSettingsPage()
{
    delete ui;
}

bool ScreenSettingsPage::AreSettingsAvailablePopup() const
{
    if(!settingAvailable)
    {
        GetMy::Instance().ToolsInst().DisplayPopup("Sorry,\nno Screen Settings (luminosity, tint) available for your ereader");
        return false;
    }
    return true;
}

void ScreenSettingsPage::OnSleep() const
{
    if (GetMy::Instance().ToolsInst().GetDeviceState() != DeviceState::fakeSleeping)
        return;

    if(settingAvailable)
        KoboPlatformExtra::SetFrontlightLevelStatic(extraFunk.getKoboDeviceExtraDescriptorStatic()->frontlightSettings.naturalLightMin, tint);
}

void ScreenSettingsPage::OnWakeUp() const
{
    if (GetMy::Instance().ToolsInst().GetDeviceState() != DeviceState::busy)
        return;

    if(settingAvailable)
        KoboPlatformExtra::SetFrontlightLevelStatic(luminosity, tint);
}

void ScreenSettingsPage::ToggleLight()
{
    lightToggleStatus = !lightToggleStatus;
    KoboPlatformExtra::SetFrontlightLevelStatic((lightToggleStatus) ? luminosity : 0, tint);
}

void ScreenSettingsPage::on_LuminositySlider_valueChanged(int value)
{
    luminosity = value;
    settingsSerializer.setValue("ScreenSettings/luminosity", value);
    ui->LuminosityValue->setText(QString::number(luminosity));
    KoboPlatformExtra::SetFrontlightLevelStatic(luminosity, tint);
}

void ScreenSettingsPage::on_TintSlider_valueChanged(int value)
{
    tint = value;
    settingsSerializer.setValue("ScreenSettings/tint", value);
    ui->TintValue->setText(QString::number(tint));
    KoboPlatformExtra::SetFrontlightLevelStatic(luminosity, tint);
}
