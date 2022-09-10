#ifndef QCMENTRYGUESS_H
#define QCMENTRYGUESS_H

#include <QWidget>
#include <QString>
#include <optional>
#include "Src/DefinesLand.h"
#include "Src/QcmDataEntry.h"

#include <QLabel>

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

    void SetGuessData(QcmDataEntry* s, QcmExerciceType qcmType, bool displayKanji, std::optional<bool> correct = std::nullopt);

    int GetMarginSumWidth() const;
    int GetMarginSumHeight() const;
    QcmDataEntry* GetSymbol() const { return symbol; }

    bool ApplyGuessTextAndCorrection(int guessWidth, int guessHeight);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void on_EntryGuess_clicked();

private:
    QcmDataEntry* symbol;
    Ui::QcmEntryGuess *ui;
    bool correctGuess;

    QcmExerciceType qcmType;

    bool sizeCorrected = false;
    int originalFntSize;
    QFont correctedFnt;
    bool displayKanji;

    static int continuousFntResizeCounter;
};

#endif // QCMENTRYGUESS_H
