#ifndef QCMENTRYGUESS_H
#define QCMENTRYGUESS_H

#include <QWidget>
#include <QString>
#include "Src/symbolstables.h"
#include "Src/qcmexercice.h"

namespace Ui
{
    class QcmEntryGuess;
}

class QcmEntryGuess : public QWidget
{
    Q_OBJECT

public:
    QcmEntryGuess() = delete;
    explicit QcmEntryGuess(QWidget *parent = nullptr);
    ~QcmEntryGuess() override;

    void SetGuess(Symbol* s, QcmExercice::QcmExerciceType qcmType, bool);
    void CorrectFontSize();
    void CorrectFontSizeBis();

    Symbol* GetSymbol() const { return symbol; }

    static void SetFontSizeWarningDisplayedState(bool fsw) { fntWarnDisplayed = fsw; }

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_EntryGuess_clicked();

private:
    Symbol* symbol;
    Ui::QcmEntryGuess *ui;
    bool correctGuess;

    static bool fntWarnDisplayed;
};

#endif // QCMENTRYGUESS_H
