#include "Src/Pages/WelcomePage.h"
#include "ui_WelcomePage.h"

WelcomePage::WelcomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomePage)
{
    ui->setupUi(this);
    ui->QrCode->setScaledContents(false);
}

void WelcomePage::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    QPixmap qrCodePix(":/pictures/QrCode/obenkyoQrCode.png");
    qrCodePix = qrCodePix.scaled(ui->QrCode->height(), ui->QrCode->height(), Qt::KeepAspectRatio);
    ui->QrCode->setPixmap(qrCodePix);
}

WelcomePage::~WelcomePage()
{
    delete ui;
}
