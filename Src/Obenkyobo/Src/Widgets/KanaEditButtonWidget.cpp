#include <QPainter>
#include "Src/Widgets/KanaEditButtonWidget.h"
#include "Src/Pages/FntSettingsPage.h"
#include "Src/GetMy.h"

KanaEditButtonWidget::KanaEditButtonWidget(QWidget* /*widget*/)
    : learningState(-1), JP(), Romanji()
{}

void KanaEditButtonWidget::setLearningState(int _learningState)
{
    learningState = MAX_LEARNING_STATE_VALUE - _learningState;
}

// Called when switching StackedWidget to "Edit [Kana] Set" page
void KanaEditButtonWidget::RefreshJpFixedSizedFnt()
{
    jpFixedSizeFnt = (kanaFamily == KanaFamilyEnum::hiragana)
                    ? QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFamily(), JP_SYMBOL_FNT_SIZE)
                    : QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFamily(), JP_SYMBOL_FNT_SIZE);
}

void KanaEditButtonWidget::paintEvent(QPaintEvent *event)
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
    int screenWidth = GetMy::Instance().Descriptor().width;
    int outerSize = screenWidth/80;
    int doubleOuterSize = outerSize*2;
    int outerThickness = screenWidth/500;
    int innerSize = outerSize - outerThickness*2.5f;
    painter.setPen(QPen((isChecked()) ? Qt::white : Qt::black, outerThickness));
    painter.drawEllipse(QPointF(width() - doubleOuterSize, doubleOuterSize), outerSize, outerSize);
    if (isChecked())
    {
        painter.setBrush(QBrush(Qt::white));
        painter.drawEllipse(QPointF(width() - doubleOuterSize, doubleOuterSize), innerSize, innerSize);
    }
}
