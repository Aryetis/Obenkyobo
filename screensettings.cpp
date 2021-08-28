#include "screensettings.h"
#include "ui_screensettings.h"

ScreenSettings::ScreenSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenSettings)
{
    ui->setupUi(this);

//    ui->LuminositySlider->SetValue();
//    ui->TintSlider->setValue();
}

ScreenSettings::~ScreenSettings()
{
    delete ui;
}

void ScreenSettings::on_LuminositySlider_valueChanged(int value)
{

}

void ScreenSettings::on_TintSlider_valueChanged(int value)
{

}
