#include "Src/Pages/ScreenSettingsPage.h"
#include "ui_ScreenSettingsPage.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"

// Note : the whole kobo family frontlight drivers only take integeter, so we're good setting sliders's step to 1
ScreenSettingsPage::ScreenSettingsPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::ScreenSettingsPage),
    luminosity(0), tint(0), settingAvailable(false),
    lightToggleStatus(true),
    desc(GetMy::Instance().Descriptor()),
    settingsSerializer(GetMy::Instance().SettingSerializerInst())
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
    {
        ui->LuminosityLabel->setVisible(false);
        ui->LuminosityValue->setVisible(false);
        ui->LuminositySlider->setVisible(false);
    }

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
    {
        ui->TintLabel->setVisible(false);
        ui->TintValue->setVisible(false);
        ui->TintSlider->setVisible(false);
    }

    if (settingAvailable)
        KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);

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
        GetMy::Instance().ToolsInst()->DisplayPopup("Sorry,\nno Screen Settings (luminosity, tint) available for your ereader");
        return false;
    }
    return true;
}

void ScreenSettingsPage::OnSleep() const
{
    if (GetMy::Instance().ToolsInst()->GetDeviceState() != DeviceState::busy)
        return;

    if(settingAvailable)
        KoboPlatformFunctions::setFrontlightLevel(desc.frontlightSettings.naturalLightMin, tint);
}

void ScreenSettingsPage::OnWakeUp() const
{
    if (GetMy::Instance().ToolsInst()->GetDeviceState() != DeviceState::busy)
        return;

    if(settingAvailable)
        KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}

void ScreenSettingsPage::ToggleLight()
{
    lightToggleStatus = !lightToggleStatus;
    KoboPlatformFunctions::setFrontlightLevel((lightToggleStatus) ? luminosity : 0, tint);
}

void ScreenSettingsPage::on_LuminositySlider_valueChanged(int value)
{
    luminosity = value;
    settingsSerializer->setValue("ScreenSettings/luminosity", value);
    ui->LuminosityValue->setText(QString::number(luminosity));
    KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}

void ScreenSettingsPage::on_TintSlider_valueChanged(int value)
{
    tint = value;
    settingsSerializer->setValue("ScreenSettings/tint", value);
    ui->TintValue->setText(QString::number(tint));
    KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}
