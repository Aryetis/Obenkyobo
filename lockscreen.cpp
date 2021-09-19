#include "lockscreen.h"
#include "ui_lockscreen.h"

LockScreen::LockScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LockScreen)
{
    ui->setupUi(this);
}

LockScreen::~LockScreen()
{
    delete ui;
}
