#include "screensettings.h"
#include "ui_screensettings.h"

ScreenSettings::ScreenSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenSettings)
{
    ui->setupUi(this);
}

ScreenSettings::~ScreenSettings()
{
    delete ui;
}
