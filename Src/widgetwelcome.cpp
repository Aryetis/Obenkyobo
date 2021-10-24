#include "Src/widgetwelcome.h"
#include "ui_widgetwelcome.h"

#include <iostream>

WidgetWelcome::WidgetWelcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetWelcome)
{
    ui->setupUi(this);
    ui->QrCode->setScaledContents(false);
}

void WidgetWelcome::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    QPixmap qrCodePix(":/pictures/QrCode/obenkyoQrCode.png");
    qrCodePix = qrCodePix.scaled(ui->QrCode->height(), ui->QrCode->height(), Qt::KeepAspectRatio);
    ui->QrCode->setPixmap(qrCodePix);
}

WidgetWelcome::~WidgetWelcome()
{
    delete ui;
}
