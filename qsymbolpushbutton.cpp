#include "qsymbolpushbutton.h"
#include <QPainter>
#include "symbolstables.h"

QSymbolPushButton::QSymbolPushButton()
    : learningState(-1)
{ }

QSymbolPushButton::QSymbolPushButton(QWidget * /*widget*/)
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
    painter.setPen(penHText);
    painter.drawText(geometry(), Qt::AlignRight | Qt::AlignBottom, QString::number(learningState)+" ");
    painter.drawEllipse(QPointF(width() - 10, 10), 5, 5);
    if (isChecked())
    {
        painter.setBrush(QBrush(QColor(255,255,255)));
        painter.drawEllipse(QPointF(width() - 10, 10), 3, 3);
    }
}
