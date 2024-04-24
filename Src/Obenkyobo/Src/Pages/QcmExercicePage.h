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
    void SetQcmConfigChanged(bool b);

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_SwitchButton_clicked();

private:
    void InitializeStemFont();
    void ApplyCorrectStemFontSize();
    void ApplyGuessesTextAndCorrection();
    void FixElementsSizes();
    bool LearningScoreTransaction(std::vector<std::pair<int, QcmDataEntry*> > transaction);

    Ui::QcmExercicePage *ui;
    QList<QcmEntryGuess*> guesses;
    int scoreCounter, errorCounter;
    QcmExerciceType currentQcmType;
    QcmDataEntry* stem; // Answer to guess
    int refreshCounter;
    QFont curHiraganaNonSized, curKatakanaNonSized, curRomanjiNonSized, curKanjiNonSized, stemFont;
    QSettings& settingsSerializer;
    bool stemDisplayKanji;
    std::vector<QcmDataEntry*> entriesPool; // DO NOT own its data => don't delete its content
    VocabDataPool* vdp;                     // holds entriesPool's data for vocab QCM (kana ones being hardcoded in KanasTables)

    bool initialPaintDone;
    bool qcmConfigChanged; // switching 1x3, 2x3, 3x3 => needs to refix elements's sizes
    static int continuousStemFntResizeCoRunter;

    int contentRectWidth;
    int contentRectHeight;
};

#endif // QCMEXERCICEPAGE_H
