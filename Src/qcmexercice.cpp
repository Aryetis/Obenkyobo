#include "Src/qcmexercice.h"
#include "ui_qcmexercice.h"
#include "Src/qcmentryguess.h"
#include "Src/fntsetting.h"
#include "Src/GetMy.h"
#include "Src/tools.h"
#include <algorithm>
#include "Src/appsettings.h"
#include "Src/mainwindow.h"
#include <QString>

QcmExercice::QcmExercice(QWidget *parent) :
    QWidget(parent), ui(new Ui::QcmExercice), scoreCounter(0), errorCounter(0),
    currentQcmType(QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM),
    refreshCounter(0), curHiraganaNonSized(), curKatakanaNonSized(), curRomanjiNonSized(), stemFont(),
    settingsSerializer(GetMy::Instance().SettingSerializer())
{
    ui->setupUi(this);

    ui->ResultLabelGroupBox->setFlat(true);
    ui->ResultLabelGroupBox->setStyleSheet("border : none; ");

    GetMy::Instance().SetQcmExerciceWidget(this);
}

QcmExercice::~QcmExercice()
{
    qDeleteAll(guesses);
    guesses.clear();
    delete ui;
}

void QcmExercice::InitializeExercice(QcmExercice::QcmExerciceType qcmType, bool newQcmRequested /* = false*/)
{
    //************************ Initialize Fonts, reset counters ************************
    if (newQcmRequested)
    {
        currentQcmType = qcmType;
        scoreCounter = 0;
        errorCounter = 0;

        int labelRightPointSize = ui->ResultLabelRight->font().pointSize();
        curHiraganaNonSized = QFont(GetMy::Instance().FntSettingWidget().GetCurrentHiraganaFamily(), labelRightPointSize);
        curKatakanaNonSized = QFont(GetMy::Instance().FntSettingWidget().GetCurrentKatakanaFamily(), labelRightPointSize);
        curRomanjiNonSized = QFont(GetMy::Instance().FntSettingWidget().GetCurrentRomanjiFamily(), labelRightPointSize);

        FntSetting& fntSetting = GetMy::Instance().FntSettingWidget();
        switch (qcmType)
        {
            case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM :
            case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_Kbd :
            case QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM :
            case QcmExercice::QcmExerciceType::Katakana_to_Romanji_Kbd :
            {
                stemFont = QFont(fntSetting.GetCurrentRomanjiFamily(), fntSetting.GetCurrentRomanjiSize() + fntSetting.GetStemBoostSize());
                break;
            }
            case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM :
            {
                stemFont = QFont(fntSetting.GetCurrentHiraganaFamily(), fntSetting.GetCurrentHiraganaSize() + fntSetting.GetStemBoostSize());
                break;
            }
            case QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM :
            {
                stemFont = QFont(fntSetting.GetCurrentKatakanaFamily(), fntSetting.GetCurrentKatakanaSize() + fntSetting.GetStemBoostSize());
                break;
            }
        }
    }

    //************************ Initialize Entries Pool ************************
    std::vector<Symbol*> entriesPool;
    SymbolsTableFamily& targetFamily = (qcmType < Katakana_to_Romanji_QCM)
            ? SymbolsTables::HiraganaSymbolsTableFamily  // hiragana QCM
            : SymbolsTables::KatakanaSymbolsTableFamily; // katakana QCM

    for (SymbolsTableSection& SymbolSection : targetFamily.Data() )
        for(Symbol& symbol : SymbolSection.Data())
            if (symbol.Enabled())
                entriesPool.push_back(&symbol);

    //************************ Initialize Shuffled Symbols Pool ************************
    std::vector<Symbol*> shuffledSymbols{};
    shuffledSymbols.reserve(entriesPool.size()-1);
    std::remove_copy(entriesPool.begin(), entriesPool.end(), std::back_inserter(shuffledSymbols), stem);
    std::shuffle(std::begin(shuffledSymbols), std::end(shuffledSymbols), Tools::GetInstance().Rng_Engine());

    //************************ Initialize Stem  ************************
    if (GetMy::Instance().AppSettingWidget().IsWeightedRandomEnabled())
    {
        std::vector<int> weights;
        for (Symbol* symbol : shuffledSymbols)
            weights.push_back(symbol->LearningState());

        std::discrete_distribution<size_t> distr(weights.begin(), weights.end());
        stem = shuffledSymbols[distr(Tools::GetInstance().MT())];
    }
    else
        stem = *Tools::GetInstance().GetRandom(shuffledSymbols.begin(), shuffledSymbols.end());

    assert(stem != nullptr);

    ui->GuessMe->setFont(stemFont);
    switch (qcmType)
    {
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_Kbd :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            ui->GuessMe->setText(stem->Romanji());
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM :
        case QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM :
        {
            ui->GuessMe->setText(stem->JP());
            break;
        }
    }

    //************************ Clearing previous board ************************
    qDeleteAll(guesses);
    guesses.clear();

    //************************ Initialize Entries board ************************
    int NbrOfEntriesLine = GetMy::Instance().AppSettingWidget().GetNumberOfEntryLine();
    int NbrOfEntriesRow = GetMy::Instance().AppSettingWidget().GetNumberOfEntryRow();
    int stemSlot = Tools::GetInstance().GetRandomInt(0, (NbrOfEntriesLine*NbrOfEntriesRow)-1);
    Symbol* joker = shuffledSymbols[static_cast<std::vector<Symbol>::size_type>(stemSlot)]; // Symbol replaced by stem
    for(int i= 0; i<NbrOfEntriesLine*NbrOfEntriesRow; ++i)
    {
        div_t entryPos = div(i, NbrOfEntriesLine);
        QcmEntryGuess* foo = new QcmEntryGuess();
        guesses.append(foo);
        Symbol* curSym = shuffledSymbols[static_cast<std::vector<Symbol>::size_type>(i)];

        if (i == stemSlot)
            foo->SetGuess(stem, currentQcmType, true);
        else
        {
            if (curSym == stem) // avoid double entries
                foo->SetGuess(joker, currentQcmType, false);
            else
                foo->SetGuess(curSym, currentQcmType, false);
        }

        ui->EntriesGridLayout->addWidget(foo, entryPos.rem, entryPos.quot);
    }

    for (QcmEntryGuess* entry : guesses)
        entry->CorrectFontSize();

    //************************ UI score error Counters ************************
    ui->ScoreCounter->setNum(scoreCounter);
    ui->ErrorsCounter->setNum(errorCounter);

    //************************ Hard Refresh ************************
    int HardRefreshFreq = GetMy::Instance().AppSettingWidget().HardRefreshFreq();
    if (HardRefreshFreq > 0 && ++refreshCounter >= HardRefreshFreq)
    {
        refreshCounter = 0;
        this->repaint();
    }
}

void QcmExercice::OnGuessClicked(bool correct, QcmEntryGuess* entryGuess)
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
        if ( EntryGuessLearningState < Symbol::GetMaxlearningState() )
            entryGuess->GetSymbol()->LearningState(EntryGuessLearningState+1);
        if ( stem->LearningState() < Symbol::GetMaxlearningState() )
            stem->LearningState(stem->LearningState()+1);
    }


    // print feedback informing of correct / incorrect choice
    switch (currentQcmType)
    {
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_Kbd :
        {
            ui->ResultLabelLeft->setFont(curRomanjiNonSized);
            ui->ResultLabelLeft->setText(stem->Romanji());
            ui->ResultLabelLeft->setStyleSheet("QLabel { border: none }");

            ui->ResultLabelMiddle->setFont(curHiraganaNonSized);
            ui->ResultLabelMiddle->setText(stem->JP());
            ui->ResultLabelMiddle->setStyleSheet("QLabel { border: none }");
            break;
        }
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            ui->ResultLabelLeft->setFont(curRomanjiNonSized);
            ui->ResultLabelLeft->setText(stem->Romanji());
            ui->ResultLabelLeft->setStyleSheet("QLabel { border: none }");

            ui->ResultLabelMiddle->setFont(curKatakanaNonSized);
            ui->ResultLabelMiddle->setText(stem->JP());
            ui->ResultLabelMiddle->setStyleSheet("QLabel { border: none }");
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM :
        {
            ui->ResultLabelLeft->setFont(curHiraganaNonSized);
            ui->ResultLabelLeft->setText(stem->JP());
            ui->ResultLabelLeft->setStyleSheet("QLabel { border: none }");

            ui->ResultLabelMiddle->setFont(curRomanjiNonSized);
            ui->ResultLabelMiddle->setText(stem->Romanji());
            ui->ResultLabelMiddle->setStyleSheet("QLabel { border: none }");
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM :
        {
            ui->ResultLabelLeft->setFont(curKatakanaNonSized);
            ui->ResultLabelLeft->setText(stem->JP());
            ui->ResultLabelLeft->setStyleSheet("QLabel { border: none }");

            ui->ResultLabelMiddle->setFont(curRomanjiNonSized);
            ui->ResultLabelMiddle->setText(stem->Romanji());
            ui->ResultLabelMiddle->setStyleSheet("QLabel { border: none }");
            break;
        }
    }

    ui->ResultLabelRight->setText((correct) ? "☑" : "☒");
    ui->ResultLabelRight->setStyleSheet("QLabel { border: none }");
    // TODO make it blink upon fail ?
    ui->ResultLabelGroupBox->setStyleSheet((correct) ? "QGroupBox { border : none }"
                                                     : "QGroupBox { border : 3px solid black }");

    InitializeExercice(currentQcmType);
}
