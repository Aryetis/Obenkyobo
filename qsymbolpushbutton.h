#ifndef QSYMBOLPUSHBUTTON_H
#define QSYMBOLPUSHBUTTON_H

#include <QPushButton>
#include "symbolsetting.h"

class QSymbolPushButton : public QPushButton
{
protected :
    void paintEvent(QPaintEvent *event) override;

public :
    QSymbolPushButton();
    QSymbolPushButton(QWidget* widget);

    void setSymbolFamilyEnum(SymbolFamilyEnum _fe) { symbolFamily = _fe; }
    void setLearningState(int learningState);
    void setJpText(QString _jp) { JP = _jp; }
    void setRmjText(QString _rmj) { Romanji = _rmj; }

private :
    SymbolFamilyEnum symbolFamily;
    int learningState; // inversed weight
    QString JP, Romanji;
};

#endif // QSYMBOLPUSHBUTTON_H
