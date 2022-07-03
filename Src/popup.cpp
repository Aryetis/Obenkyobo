#include "popup.h"
#include "ui_popup.h"

#include "GetMy.h"

#define POPUP_BORDER_SIZE 5

Popup::Popup(QString message, bool fullscreen, QWidget *parent/* = nullptr*/) : QDialog(parent), ui(new Ui::Popup)
{
    ui->setupUi(this);
    setFixedWidth(GetMy::Instance().Descriptor().width*0.9f);
    setStyleSheet(QString("QDialog{border: %1px solid black;} ").arg(POPUP_BORDER_SIZE));
    ui->message->setText(message);
    if (fullscreen)
    {
        setFixedWidth(GetMy::Instance().Descriptor().width);
        setFixedHeight(GetMy::Instance().Descriptor().height);
    }
}

Popup::~Popup()
{
    delete ui;
}

void Popup::on_OkButton_clicked()
{
    close();
}
