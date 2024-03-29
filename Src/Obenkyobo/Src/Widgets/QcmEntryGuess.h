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

    QLabel const* GetLabel() const;
    QcmDataEntry* GetDataEntry() const { return symbol; }

    void SetGuessData(QcmDataEntry* s, QcmExerciceType qcmType, bool displayKanji, std::optional<bool> correct = std::nullopt);
    static void SetContinuousFntResizeCounter(int i);

    bool ApplyGuessTextAndCorrection(int guessWidth, int guessHeight);

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals :
    void EntryGuessClickedSignal();

private slots:
    void On_EntryGuess_clicked();

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
