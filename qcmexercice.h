#ifndef QCMEXERCICE_H
#define QCMEXERCICE_H

#include <QWidget>

namespace Ui
{
    class QcmExercice;
}

class QcmEntryGuess;

class QcmExercice final : public QWidget
{
    Q_OBJECT

public:
    enum QcmExerciceType
    {
        Hiragana_to_Romanji_QCM,
        Romanji_to_Hiragana_QCM,
        Hiragana_to_Romanji_Kbd,
        Katakana_to_Romanji_QCM,
        Romanji_to_Katakana_QCM,
        Katakana_to_Romanji_Kbd
    };

    explicit QcmExercice(QWidget* parent = nullptr);
    ~QcmExercice() override;

    void InitializeExercice(QcmExercice::QcmExerciceType qcmType);
    void OnGuessClicked(bool correct);

private:
    Ui::QcmExercice *ui;
    QList<QcmEntryGuess*> guesses;
    int scoreCounter, errorCounter;
    QcmExercice::QcmExerciceType currentQcmType;
};

#endif // QCMEXERCICE_H
