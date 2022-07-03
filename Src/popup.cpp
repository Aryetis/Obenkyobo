#include "popup.h"
#include "ui_popup.h"

#include "GetMy.h"

#include <QFontMetrics>
#include <QRect>

#define POPUP_BORDER_SIZE 5
#define POPUP_MARGIN_X 0.04f
#define POPUP_MARGIN_Y 0.02f
#define POPUP_WIDTH 0.9f

Popup::Popup(QString message, bool fullscreen, QWidget *parent/* = nullptr*/) : QDialog(parent), ui(new Ui::Popup)
{
    ui->setupUi(this);
    setStyleSheet(QString("QDialog{border: %1px solid black;} ")
                  .arg(POPUP_BORDER_SIZE));

    float test = GetMy::Instance().Descriptor().height*POPUP_MARGIN_Y;
    layout()->setContentsMargins(0, test,0, test);

    ui->message->setText(message);
    ui->message->setStyleSheet(
                QString("QLabel {margin-left: %1px; margin-right: %1px}")
                .arg(GetMy::Instance().Descriptor().width*POPUP_MARGIN_X));
    ui->message->setText(message);
    if (fullscreen)
    {
        setFixedWidth(GetMy::Instance().Descriptor().width);
        setFixedHeight(GetMy::Instance().Descriptor().height);
    }
    else
    {
        // note : margin are drawn toward the zone
        float width = GetMy::Instance().Descriptor().width * POPUP_WIDTH;
        setFixedWidth(width);
        float widthTestZone = width - GetMy::Instance().Descriptor().width*POPUP_MARGIN_X*2 - POPUP_BORDER_SIZE*2;
        QFontMetrics fm(font());
        float height = fm.boundingRect(QRect(0,0,widthTestZone,100), Qt::TextWordWrap, message).height();
        ui->message->setFixedHeight(height);
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
