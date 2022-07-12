#include <algorithm>
#include <QString>
#include "Src/Pages/QcmExercicePage.h"
#include "ui_QcmExercicePage.h"
#include "Src/Widgets/QcmEntryGuess.h"
#include "Src/Pages/FntSettingsPage.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/mainwindow.h"
#include "Src/KanasTables.h"
#include "Src/DefinesLand.h"

QcmExercicePage::QcmExercicePage(QWidget *parent) :
    QWidget(parent), ui(new Ui::QcmExercicePage), scoreCounter(0), errorCounter(0),
    currentQcmType(),
    refreshCounter(0), curHiraganaNonSized(), curKatakanaNonSized(), curRomanjiNonSized(), stemFont(),
    settingsSerializer(GetMy::Instance().SettingSerializerInst())
{
    ui->setupUi(this);

    ui->ResultLabelGroupBox->setFlat(true);
    ui->ResultLabelGroupBox->setStyleSheet("border : none; ");

    GetMy::Instance().SetQcmExercicePageInst(this);
}

QcmExercicePage::~QcmExercicePage()
{
    qDeleteAll(guesses);
    guesses.clear();
    delete ui;
}

void QcmExercicePage::InitializeExercice(QcmExerciceType qcmType, bool newQcmRequested /* = false*/)
{
    //************************ Initialize Fonts, reset counters ************************
    if (newQcmRequested)
    {
        scoreCounter = 0;
        errorCounter = 0;

        QcmEntryGuess::SetFontSizeWarningDisplayedState(false);

        int labelRightPointSize = ui->ResultLabelRight->font().pointSize();
        curHiraganaNonSized = QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFamily(), labelRightPointSize);
        curKatakanaNonSized = QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFamily(), labelRightPointSize);
        curRomanjiNonSized = QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentRomanjiFamily(), labelRightPointSize);

        FntSettingsPage& fntSetting = GetMy::Instance().FntSettingsPageInst();
        switch (qcmType)
        {
            case QcmExerciceType::Hiragana_to_Romanji_MCQ :
            case QcmExerciceType::Hiragana_to_Romanji_Kbd :
            case QcmExerciceType::Katakana_to_Romanji_MCQ :
            case QcmExerciceType::Katakana_to_Romanji_Kbd :
            {
                if (newQcmRequested && (ui->SwitchButton->isVisible() || !currentQcmType.has_value()))
                    ui->SwitchButton->setVisible(false);
                stemFont = QFont(fntSetting.GetCurrentRomanjiFamily(), fntSetting.GetStemSize());
                break;
            }
            case QcmExerciceType::Romanji_to_Hiragana_MCQ :
            {
                if (newQcmRequested && (ui->SwitchButton->isVisible() || !currentQcmType.has_value()))
                    ui->SwitchButton->setVisible(false);
                stemFont = QFont(fntSetting.GetCurrentHiraganaFamily(), fntSetting.GetStemSize());
                break;
            }
            case QcmExerciceType::Romanji_to_Katakana_MCQ :
            {
                if (newQcmRequested && (ui->SwitchButton->isVisible() || !currentQcmType.has_value()))
                    ui->SwitchButton->setVisible(false);
                stemFont = QFont(fntSetting.GetCurrentKatakanaFamily(), fntSetting.GetStemSize());
                break;
            }
            case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
            case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
            {
            if (newQcmRequested && (!ui->SwitchButton->isVisible() || !currentQcmType.has_value()))
                    ui->SwitchButton->setVisible(true);
                break;
            }
        }
        currentQcmType = qcmType;
    }

    //************************ Initialize Entries Pool ************************
    std::vector<QcmDataEntry*> entriesPool;
    KanasTableFamily& targetFamily = (qcmType < Katakana_to_Romanji_MCQ)
            ? KanasTables::HiraganaSymbolsTableFamily  // hiragana QCM
            : KanasTables::KatakanaSymbolsTableFamily; // katakana QCM
    for (SymbolsTableSection& SymbolSection : targetFamily.Data() )
        for(QcmDataEntry& symbol : SymbolSection.Data())
            if (symbol.IsEnabled())
                entriesPool.push_back(&symbol);

    //************************ Initialize Shuffled Symbols Pool ************************
    std::vector<QcmDataEntry*> shuffledSymbols{};
    shuffledSymbols.reserve(entriesPool.size()-1);
    std::remove_copy(entriesPool.begin(), entriesPool.end(), std::back_inserter(shuffledSymbols), stem);
    std::shuffle(std::begin(shuffledSymbols), std::end(shuffledSymbols), GetMy::Instance().ToolsInst()->Rng_Engine());

    //************************ Initialize Stem  ************************
    if (GetMy::Instance().AppSettingsPageInst().IsWeightedRandomEnabled())
    {
        std::vector<int> weights;
        for (QcmDataEntry* symbol : shuffledSymbols)
            weights.push_back(symbol->LearningState());

        std::discrete_distribution<size_t> distr(weights.begin(), weights.end());
        stem = shuffledSymbols[distr(GetMy::Instance().ToolsInst()->MT())];
    }
    else
        stem = *GetMy::Instance().ToolsInst()->GetRandom(shuffledSymbols.begin(), shuffledSymbols.end());

    assert(stem != nullptr);

    ui->GuessMe->setFont(stemFont);
    switch (qcmType)
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            ui->GuessMe->setText(*stem->Romanji());
            break;
        }
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            ui->GuessMe->setText(*stem->Kanas());
            break;
        }
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
        {
            // TODO NOW
            break;
        }
    }

    //************************ Clearing previous board ************************
    qDeleteAll(guesses);
    guesses.clear();

    //************************ Initialize Entries board ************************
    int NbrOfEntriesLine = GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryLine();
    int NbrOfEntriesRow = GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryRow();
    int stemSlot = GetMy::Instance().ToolsInst()->GetRandomInt(0, (NbrOfEntriesLine*NbrOfEntriesRow)-1);
    QcmDataEntry* joker = shuffledSymbols[static_cast<std::vector<QcmDataEntry>::size_type>(stemSlot)]; // Symbol replaced by stem
    for(int i= 0; i<NbrOfEntriesLine*NbrOfEntriesRow; ++i)
    {
        div_t entryPos = div(i, NbrOfEntriesLine);
        QcmEntryGuess* foo = new QcmEntryGuess(this); // TODO : don't new, reuse previous ones a la VocabularyDisplayPage (be careful with qcmSize setting too)
        guesses.append(foo);
        QcmDataEntry* curSym = shuffledSymbols[static_cast<std::vector<QcmDataEntry>::size_type>(i)];

        if (i == stemSlot)
            foo->SetGuess(stem, currentQcmType.value(), true);
        else
        {
            if (curSym == stem) // avoid double entries
                foo->SetGuess(joker, currentQcmType.value(), false);
            else
                foo->SetGuess(curSym, currentQcmType.value(), false);
        }

        ui->EntriesGridLayout->addWidget(foo, entryPos.rem, entryPos.quot);
    }

    //************************ UI score error Counters ************************
    ui->ScoreCounter->setNum(scoreCounter);
    ui->ErrorsCounter->setNum(errorCounter);

    //************************ Hard Refresh ************************
    int HardRefreshFreq = GetMy::Instance().AppSettingsPageInst().GetHardRefreshFreq();
    if (HardRefreshFreq > 0 && ++refreshCounter >= HardRefreshFreq)
    {
        refreshCounter = 0;
        GetMy::Instance().MainWindowInst().AggressiveClearScreen(true);
    }
}

void QcmExercicePage::OnGuessClicked(bool correct, QcmEntryGuess* entryGuess)
{
    std::cout << "LOG : QcmExercice::OnGuessClicked()" << std::endl;
    if ( correct )
    {
        ++scoreCounter;
        int appStatisticsScore = settingsSerializer->value("AppStatistics/score", 0).toInt();
        settingsSerializer->setValue("AppStatistics/score", ++appStatisticsScore);
        int EntryGuessLearningState = entryGuess->GetSymbol()->LearningState();
        if ( EntryGuessLearningState > 0 )
            entryGuess->GetSymbol()->LearningState(EntryGuessLearningState-1);
    }
    else
    {
        ++errorCounter;
        int appStatisticsError = settingsSerializer->value("AppStatistics/error", 0).toInt();
        settingsSerializer->setValue("AppStatistics/error", ++appStatisticsError);
        int EntryGuessLearningState = entryGuess->GetSymbol()->LearningState();
        if ( EntryGuessLearningState < MAX_LEARNING_STATE_VALUE )
            entryGuess->GetSymbol()->LearningState(EntryGuessLearningState+1);
        if ( stem->LearningState() < MAX_LEARNING_STATE_VALUE )
            stem->LearningState(stem->LearningState()+1);
    }


    // print feedback informing of correct / incorrect choice // TODO use the correct font for the jp part
    ui->ResultLabelLeft->setStyleSheet("QLabel { border: none }");
    ui->ResultLabelMiddle->setStyleSheet("QLabel { border: none }");
    switch (currentQcmType.value())
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        {
            ui->ResultLabelLeft->setFont(curRomanjiNonSized);
            ui->ResultLabelLeft->setText(*stem->Romanji());

            ui->ResultLabelMiddle->setFont(curHiraganaNonSized);
            ui->ResultLabelMiddle->setText(*stem->Kanas());
            break;
        }
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            ui->ResultLabelLeft->setFont(curRomanjiNonSized);
            ui->ResultLabelLeft->setText(*stem->Romanji());

            ui->ResultLabelMiddle->setFont(curKatakanaNonSized);
            ui->ResultLabelMiddle->setText(*stem->Kanas());
            break;
        }
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        {
            ui->ResultLabelLeft->setFont(curHiraganaNonSized);
            ui->ResultLabelLeft->setText(*stem->Kanas());

            ui->ResultLabelMiddle->setFont(curRomanjiNonSized);
            ui->ResultLabelMiddle->setText(*stem->Romanji());
            break;
        }
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            ui->ResultLabelLeft->setFont(curKatakanaNonSized);
            ui->ResultLabelLeft->setText(*stem->Kanas());

            ui->ResultLabelMiddle->setFont(curRomanjiNonSized);
            ui->ResultLabelMiddle->setText(*stem->Romanji());
            break;
        }
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        {
            // TODO NOW
            break;
        }
        case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
        {
            // TODO NOW
            break;
        }
    }

    ui->ResultLabelRight->setText((correct) ? "☑" : "☒");
    ui->ResultLabelRight->setStyleSheet("QLabel { border: none }");
    // TODO make it blink upon fail ?
    ui->ResultLabelGroupBox->setStyleSheet((correct) ? "QGroupBox { border : none }"
                                                     : "QGroupBox { border : 5px solid black }");

    InitializeExercice(currentQcmType.value());
}
