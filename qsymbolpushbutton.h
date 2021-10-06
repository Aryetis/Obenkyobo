#ifndef QSYMBOLPUSHBUTTON_H
#define QSYMBOLPUSHBUTTON_H

#include <QPushButton>

class QSymbolPushButton : public QPushButton
{
protected :
    void paintEvent(QPaintEvent *event) override;

public :
    QSymbolPushButton();
    QSymbolPushButton(QWidget* widget);

    void setLearningState(int learningState);

private :
    int learningState;
};

#endif // QSYMBOLPUSHBUTTON_H
