#ifndef QCMEXERCICE_H
#define QCMEXERCICE_H

#include <QWidget>
#include "qcmentryguess.h"

namespace Ui
{
    class QcmExercice;
}

class QcmExercice : public QWidget
{
    Q_OBJECT

public:
    static QcmExercice& GetInstance()
    {
        static QcmExercice instance;
        return instance;
    }
    ~QcmExercice();

    void InitializeExercice();
    void OnGuessClicked(bool correct);

private:
    explicit QcmExercice(QWidget* parent = nullptr);

    Ui::QcmExercice *ui;
    QList<QcmEntryGuess*> guesses;
    int scoreCounter, errorCounter;

};

#endif // QCMEXERCICE_H
