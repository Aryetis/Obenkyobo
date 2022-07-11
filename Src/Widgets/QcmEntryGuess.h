#ifndef QCMENTRYGUESS_H
#define QCMENTRYGUESS_H

#include <QWidget>
#include <QString>
#include "Src/DefinesLand.h"
#include "Src/QcmDataEntry.h"

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

    void SetGuess(QcmDataEntry* s, QcmExerciceType qcmType, bool);
    void CorrectFontSize();
    void CorrectFontSizeBis();

    QcmDataEntry* GetSymbol() const { return symbol; }

    static void SetFontSizeWarningDisplayedState(bool fsw) { fntWarnDisplayed = fsw; }

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_EntryGuess_clicked();

private:
    QcmDataEntry* symbol;
    Ui::QcmEntryGuess *ui;
    bool correctGuess;

    static bool fntWarnDisplayed;
    QcmTypeEnum qcmSubType;
};

#endif // QCMENTRYGUESS_H
