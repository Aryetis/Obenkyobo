#include "qcmexercice.h"
#include "ui_qcmexercice.h"
#include "qcmentryguess.h"
#include "fntsetting.h"
#include "tools.h"
#include "GetMy.h"
#include <algorithm>
#include "appsettings.h"

QcmExercice::QcmExercice(QWidget *parent) :
    QWidget(parent), ui(new Ui::QcmExercice), scoreCounter(0), errorCounter(0),
    currentQcmType(QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM),
    settingsSerializer(GetMy::GetInstance().SettingSerializer())
{
    ui->setupUi(this);
    GetMy::GetInstance().SetQcmExerciceWidget(this);
}

QcmExercice::~QcmExercice()
{
    qDeleteAll(guesses);
    guesses.clear();
    delete ui;
}

void QcmExercice::InitializeExercice(QcmExercice::QcmExerciceType qcmType, bool newQcmRequested /* = false*/)
{
    if (newQcmRequested)
    {
        currentQcmType = qcmType;
        scoreCounter = 0;
        errorCounter = 0;
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
    std::shuffle(std::begin(shuffledSymbols), std::end(shuffledSymbols), Tools::rng_engine);

    //************************ Initialize Stem  ************************
    if (GetMy::GetInstance().AppSettingWidget().IsWeightedRandomEnabled())
    {
        std::vector<int> weights;
        for (Symbol* symbol : shuffledSymbols)
            weights.push_back(symbol->LearningState());

        std::discrete_distribution<size_t> distr(weights.begin(), weights.end());
        stem = shuffledSymbols[distr(Tools::mt)];
    }
    else
        stem = *Tools::GetRandom(shuffledSymbols.begin(), shuffledSymbols.end());

    assert(stem != nullptr);

    FntSetting& fntSetting = GetMy::GetInstance().FntSettingWidget();
    switch (qcmType)
    {
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_Kbd :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            QFont stemFont = fntSetting.GetCurrentRomanjiFnt();
            stemFont.setPixelSize(stemFont.pixelSize() + fntSetting.GetStemBoostSize());
            ui->GuessMe->setFont(stemFont);
            ui->GuessMe->setText(stem->Romanji());
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM :
        {
            QFont stemFont = fntSetting.GetCurrentHiraganaFnt();
            stemFont.setPixelSize(stemFont.pixelSize() + fntSetting.GetStemBoostSize());
            ui->GuessMe->setFont(stemFont);
            ui->GuessMe->setText(stem->JP());
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM :
        {
            QFont stemFont = fntSetting.GetCurrentKatakanaFnt();
            stemFont.setPixelSize(stemFont.pixelSize() + fntSetting.GetStemBoostSize());
            ui->GuessMe->setFont(stemFont);
            ui->GuessMe->setText(stem->JP());
            break;
        }
    }

    //************************ Clearing previous board ************************
    qDeleteAll(guesses);
    guesses.clear();

    //************************ Initialize Entries board ************************
    int NbrOfEntriesLine = GetMy::GetInstance().AppSettingWidget().GetNumberOfEntryLine();
    int NbrOfEntriesRow = GetMy::GetInstance().AppSettingWidget().GetNumberOfEntryRow();
    int stemSlot = Tools::GetRandomInt(0, (NbrOfEntriesLine*NbrOfEntriesRow)-1);
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
}

void QcmExercice::OnGuessClicked(bool correct, QcmEntryGuess* entryGuess)
{
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
        {
            entryGuess->GetSymbol()->LearningState(EntryGuessLearningState+1);
        }
        if ( stem->LearningState() < Symbol::GetMaxlearningState() )
        {
            stem->LearningState(stem->LearningState()+1);
        }
    }

    // TODO now why you doing nothing
//    KoboPlatformFunctions::clearScreen(true);

//    KoboPlatformFunctions::doManualRefresh(QRect(QPoint(0,0),
//        QPoint(GetMy::GetInstance().Descriptor().width, GetMy::GetInstance().Descriptor().height)));


    // TODO feedback and print previous answer in upper right counter ?
    InitializeExercice(currentQcmType);
}
