#include "screensettings.h"
#include "ui_screensettings.h"
#include "koboplatformfunctions.h"

ScreenSettings::ScreenSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenSettings),
    luminosity(),
    tint()
{
    ui->setupUi(this);

// TODO NOW initialize value from system file
//    ui->LuminositySlider->SetValue();
//    ui->TintSlider->setValue();
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
    tint = value/10;
    KoboPlatformFunctions::setFrontlightLevel(luminosity, tint);
}

//KoboDeviceDescriptor koboDevice = KoboPlatformFunctions::getKoboDeviceDescriptor();
