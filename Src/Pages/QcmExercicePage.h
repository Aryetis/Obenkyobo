#ifndef QCMEXERCICEPAGE_H
#define QCMEXERCICEPAGE_H

#include <QWidget>
#include <QSettings>
#include <optional>
#include "Src/DefinesLand.h"

namespace Ui
{
    class QcmExercicePage;
}

class QcmEntryGuess;
class QcmDataEntry;
class QcmExercicePage final : public QWidget
{
    Q_OBJECT

public:

    explicit QcmExercicePage(QWidget* parent = nullptr);
    ~QcmExercicePage() override;

    void InitializeExercice(QcmExerciceType qcmType, bool newQcmRequested = false);
    void OnGuessClicked(bool correct, QcmEntryGuess* entryGuess);

private:
    Ui::QcmExercicePage *ui;
    QList<QcmEntryGuess*> guesses;
    int scoreCounter, errorCounter;
    std::optional<QcmExerciceType> currentQcmType;
    QcmDataEntry* stem; // Answer to guess
    int refreshCounter;
    QFont curHiraganaNonSized, curKatakanaNonSized, curRomanjiNonSized, stemFont;
    QSettings* settingsSerializer;
};

#endif // QCMEXERCICEPAGE_H
