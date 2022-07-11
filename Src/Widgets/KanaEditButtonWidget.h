#ifndef KANAEDITBUTTONWIDGET_H
#define KANAEDITBUTTONWIDGET_H

#include <QPushButton>
#include "Src/DefinesLand.h"

class KanaEditButtonWidget : public QPushButton
{
protected :
    void paintEvent(QPaintEvent *event) override;

public :
    KanaEditButtonWidget() = delete;
    KanaEditButtonWidget(QWidget* widget);

    void setSymbolFamilyEnum(KanaFamilyEnum _fe) { kanaFamily = _fe; }
    void setLearningState(int learningState);
    void setJpText(QString _jp) { JP = _jp; }
    void setRmjText(QString _rmj) { Romanji = _rmj; }
    static void RefreshJpFixedSizedFnt();

private :
    inline static KanaFamilyEnum kanaFamily;
    int learningState; // inversed weight
    QString JP, Romanji;
    inline static QFont jpFixedSizeFnt;
};

#endif // KANAEDITBUTTONWIDGET_H
