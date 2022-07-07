#ifndef QCMEXERCICEPAGE_H
#define QCMEXERCICEPAGE_H

#include "Src/KanasTables.h"
#include <QWidget>
#include <QSettings>
#include <optional>

namespace Ui
{
    class QcmExercicePage;
}

class QcmEntryGuess;
class QcmExercicePage final : public QWidget
{
    Q_OBJECT

public:
    enum QcmExerciceType
    {
        Hiragana_to_Romanji_MCQ,
        Romanji_to_Hiragana_MCQ,
        Hiragana_to_Romanji_Kbd,
        Katakana_to_Romanji_MCQ,
        Romanji_to_Katakana_MCQ,
        Katakana_to_Romanji_Kbd,
        Vocabulary_to_Romanji_MCQ,
        Romanji_to_Vocabulary_MCQ
    };

    explicit QcmExercicePage(QWidget* parent = nullptr);
    ~QcmExercicePage() override;

    void InitializeExercice(QcmExercicePage::QcmExerciceType qcmType, bool newQcmRequested = false);
    void OnGuessClicked(bool correct, QcmEntryGuess* entryGuess);

private:
    Ui::QcmExercicePage *ui;
    QList<QcmEntryGuess*> guesses;
    int scoreCounter, errorCounter;
    std::optional<QcmExercicePage::QcmExerciceType> currentQcmType;
    Kana* stem; // Answer to guess
    int refreshCounter;
    QFont curHiraganaNonSized, curKatakanaNonSized, curRomanjiNonSized, stemFont;
    QSettings* settingsSerializer;
};

#endif // QCMEXERCICEPAGE_H
