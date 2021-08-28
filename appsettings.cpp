#include "appsettings.h"
#include "ui_appsettings.h"

AppSettings::AppSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppSettings)
{
    ui->setupUi(this);
}

AppSettings::~AppSettings()
{
    delete ui;
}
