#ifndef QCMEXERCICEPAGE_H
#define QCMEXERCICEPAGE_H

#include <QWidget>
#include <QSettings>
#include <optional>
#include "Src/VocabularyParser.h"
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

    bool InitializeExercice(QcmExerciceType qcmType, bool newQcmRequested = false);
    void OnGuessClicked(bool correct, QcmEntryGuess* entryGuess);

protected:
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_SwitchButton_clicked();

private:
    void CheckContinuousFntResizeCounter();

    Ui::QcmExercicePage *ui;
    QList<QcmEntryGuess*> guesses;
    int scoreCounter, errorCounter;
    std::optional<QcmExerciceType> currentQcmType;
    QcmDataEntry* stem; // Answer to guess
    int refreshCounter;
    QFont curHiraganaNonSized, curKatakanaNonSized, curRomanjiNonSized, curKanjiNonSized, stemFont;
    QSettings* settingsSerializer;
    bool displayKanji;
    std::vector<QcmDataEntry*> entriesPool; // DO NOT own its data => don't delete its content
    VocabDataPool* vdp;                     // holds entriesPool's data for vocab QCM (kana ones being hardcoded in KanasTables)

    QFont correctedStemFnt;
    static int continuousFntResizeCounter;
};

#endif // QCMEXERCICEPAGE_H
