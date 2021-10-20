#include "Src/widgetwelcome.h"
#include "ui_widgetwelcome.h"

WidgetWelcome::WidgetWelcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetWelcome)
{
    ui->setupUi(this);
}

void WidgetWelcome::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    QPixmap qrCodePix(":/pictures/QrCode/obenkyoQrCode.png");
    qrCodePix.scaled(ui->QrCode->height(), ui->QrCode->height(), Qt::KeepAspectRatio);
    ui->QrCode->setPixmap(qrCodePix);
}

WidgetWelcome::~WidgetWelcome()
{
    delete ui;
}
