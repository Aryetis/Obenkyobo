#ifndef QCMEXERCICE_H
#define QCMEXERCICE_H

#include <QWidget>
#include "qcmentryguess.h"

namespace Ui {
class QcmExercice;
}

class QcmExercice : public QWidget
{
    Q_OBJECT

public:
    explicit QcmExercice(QWidget *parent = nullptr);
    ~QcmExercice();
    void InitializeExercice();
private:
    Ui::QcmExercice *ui;
    QList<QcmEntryGuess*> guesses;
};

#endif // QCMEXERCICE_H
