#ifndef QCMENTRYGUESS_H
#define QCMENTRYGUESS_H

#include <QWidget>
#include <QString>
#include "Src/KanasTables.h"
#include "Src/Pages/QcmExercicePage.h"

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

    void SetGuess(Kana* s, QcmExercicePage::QcmExerciceType qcmType, bool);
    void CorrectFontSize();
    void CorrectFontSizeBis();

    Kana* GetSymbol() const { return symbol; }

    static void SetFontSizeWarningDisplayedState(bool fsw) { fntWarnDisplayed = fsw; }

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_EntryGuess_clicked();

private:
    Kana* symbol;
    Ui::QcmEntryGuess *ui;
    bool correctGuess;

    static bool fntWarnDisplayed;
    enum QcmTypeEnum
    {
        KanaToRmj,
        RmjToKana
    };
    QcmTypeEnum qcmSubType;
};

#endif // QCMENTRYGUESS_H
