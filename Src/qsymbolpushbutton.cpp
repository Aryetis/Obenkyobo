#include "Src/qsymbolpushbutton.h"
#include <QPainter>
#include "Src/symbolstables.h"
#include "Src/fntsetting.h"

#define JP_SYMBOL_FNT_SIZE 20

QSymbolPushButton::QSymbolPushButton(QWidget* /*widget*/)
    : learningState(-1), JP(), Romanji()
{ }

void QSymbolPushButton::setLearningState(int _learningState)
{
    learningState = MAX_LEARNING_STATE_VALUE - _learningState;
}

// Called when switching StackedWidget to "Edit [Kana] Set" page
void QSymbolPushButton::RefreshJpFixedSizedFnt()
{
    jpFixedSizeFnt = (symbolFamily == SymbolFamilyEnum::hiragana)
                    ? QFont(GetMy::Instance().FntSettingWidget().GetCurrentHiraganaFamily(), JP_SYMBOL_FNT_SIZE)
                    : QFont(GetMy::Instance().FntSettingWidget().GetCurrentKatakanaFamily(), JP_SYMBOL_FNT_SIZE);
}

void QSymbolPushButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    QPen penHText(isChecked() ? Qt::white : Qt::black);

    // Drawing Learning State
    painter.setPen(penHText);
    painter.drawText(geometry(), Qt::AlignRight | Qt::AlignBottom, QString::number(learningState)+" ");

    // Drawing Romanji text (don't use GetCurrentRomanjiFamily font, I'm too lazy to correct their size in frontforge)
    painter.drawText(geometry(), Qt::AlignCenter | Qt::AlignBottom, Romanji);

    // Drawing Jp text (do it better)
    jpFixedSizeFnt.setPixelSize(sizeHint().height());
    painter.setFont(jpFixedSizeFnt);
    painter.drawText(QRect(0, 0, width(), height()/2), Qt::AlignCenter, JP);

    // Drawing Enabled Status
    int outerSize = height()/10;
    int doubleOuterSize = outerSize*2;
    int innerSize = outerSize * 0.75f;
    painter.setPen(QPen((isChecked()) ? Qt::white : Qt::black, 2));
    painter.drawEllipse(QPointF(width() - doubleOuterSize, doubleOuterSize), outerSize, outerSize);
    if (isChecked())
    {
        painter.setBrush(QBrush(Qt::white));
        painter.drawEllipse(QPointF(width() - doubleOuterSize, doubleOuterSize), innerSize, innerSize);
    }
}
