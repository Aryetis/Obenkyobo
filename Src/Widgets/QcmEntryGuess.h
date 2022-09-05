#ifndef QCMENTRYGUESS_H
#define QCMENTRYGUESS_H

#include <QWidget>
#include <QString>
#include <optional>
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

    void SetGuess(QcmDataEntry* s, QcmExerciceType qcmType, bool displayKanji, std::optional<bool> correct = std::nullopt);

    int GetMarginSumWidth() const;
    int GetMarginSumHeight() const;
    QcmDataEntry* GetSymbol() const { return symbol; }

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void on_EntryGuess_clicked();

private:
    void ComputeSizeCorrection();

    QcmDataEntry* symbol;
    Ui::QcmEntryGuess *ui;
    bool correctGuess;

    QcmExerciceType qcmType;

    bool sizeCorrected = false;
    int originalFntSize;
    QFont correctedFnt;
    bool displayKanji;

    inline static std::optional<float> vanillaWidth = std::nullopt, vanillaHeight = std::nullopt;
    bool spawning;

    static int continuousFntResizeCounter;
};

#endif // QCMENTRYGUESS_H
