#include "widgetwelcome.h"
#include "ui_widgetwelcome.h"

WidgetWelcome::WidgetWelcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetWelcome)
{
    ui->setupUi(this);
}

WidgetWelcome::~WidgetWelcome()
{
    delete ui;
}
