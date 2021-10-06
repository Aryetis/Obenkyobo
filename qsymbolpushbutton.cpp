#include "qsymbolpushbutton.h"
#include <QPainter>

QSymbolPushButton::QSymbolPushButton()
    : learningState(-1)
{ }

QSymbolPushButton::QSymbolPushButton(QWidget * /*widget*/)
{ }

void QSymbolPushButton::setLearningState(int _learningState)
{
    learningState = _learningState;
}

void QSymbolPushButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);

    // TODO make everything responsive, based upon width() height()
    QPen penHText(isChecked() ? QColor(255, 255, 255) : QColor(0, 0, 0));
    painter.setPen(penHText);
    painter.drawText(geometry(), Qt::AlignRight | Qt::AlignBottom, QString::number(learningState)+" ");
    painter.drawEllipse(QPointF(width() - 15, 15), 5, 5);
    if (isChecked())
    {
        painter.setBrush(QBrush(QColor(255,255,255)));
        painter.drawEllipse(QPointF(width() - 15, 15), 3, 3);
    }
}
