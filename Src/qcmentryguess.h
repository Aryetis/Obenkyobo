#ifndef QCMENTRYGUESS_H
#define QCMENTRYGUESS_H

#include <QWidget>
#include <QString>
#include "symbolstables.h"
#include "qcmexercice.h"

namespace Ui
{
    class QcmEntryGuess;
}

class QcmEntryGuess : public QWidget
{
    Q_OBJECT

public:
    explicit QcmEntryGuess(QWidget *parent = nullptr);
    ~QcmEntryGuess() override;

    void SetGuess(Symbol* s, QcmExercice::QcmExerciceType qcmType, bool);
    void CorrectFontSize();

    Symbol* GetSymbol() const { return symbol; }

private slots:
    void on_EntryGuess_clicked();

private:
    Symbol* symbol;
    Ui::QcmEntryGuess *ui;
    bool correctGuess;
};

#endif // QCMENTRYGUESS_H
