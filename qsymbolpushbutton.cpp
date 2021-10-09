#include "qsymbolpushbutton.h"
#include <QPainter>

#include "symbolstables.h"
#include "fntsetting.h"

#include <QTextDocument>
#include <QSize>

QSymbolPushButton::QSymbolPushButton()
    : symbolFamily(), learningState(-1), JP(), Romanji()
{ }

QSymbolPushButton::QSymbolPushButton(QWidget* /*widget*/)
{ }

void QSymbolPushButton::setLearningState(int _learningState)
{
    learningState = MAX_LEARNING_STATE_VALUE - _learningState;
}

void QSymbolPushButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    QPen penHText(isChecked() ? QColor(255, 255, 255) : QColor(0, 0, 0));

    // Drawing Learning State
    painter.setPen(penHText);
    painter.drawText(geometry(), Qt::AlignRight | Qt::AlignBottom, QString::number(learningState)+" ");

    // Drawing Romanji text (don't use GetCurrentRomanjiFamily font, I'm too lazy to correct their size in frontforge)
//    painter.drawText(QRect(0, height()/2, width(), height()/2), Qt::AlignCenter, Romanji);
    painter.drawText(geometry(), Qt::AlignCenter | Qt::AlignBottom, Romanji);

    // Drawing Jp text
    QFont jpNonResizedFnt = (symbolFamily == SymbolFamilyEnum::hiragana)
            ? QFont(GetMy::Instance().FntSettingWidget().GetCurrentHiraganaFamily(), 20)
            : QFont(GetMy::Instance().FntSettingWidget().GetCurrentKatakanaFamily(), 20);
    painter.setFont(jpNonResizedFnt);
    painter.drawText(QRect(0, 0, width(), height()/2), Qt::AlignCenter, JP);


    // Drawing Enabled Status
    painter.drawEllipse(QPointF(width() - 10, 10), 5, 5);
    if (isChecked())
    {
        painter.setBrush(QBrush(QColor(255,255,255)));
        painter.drawEllipse(QPointF(width() - 10, 10), 3, 3);
    }
}
