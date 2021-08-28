#include "fntsetting.h"
#include "ui_fntsetting.h"

FntSetting::FntSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FntSetting)
{
    ui->setupUi(this);
}

FntSetting::~FntSetting()
{
    delete ui;
}
