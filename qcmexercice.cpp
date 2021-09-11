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
    std::vector<SymbolsTableSection>& symbolsSections = (qcmType < Katakana_to_Romanji_QCM)
            ? SymbolsTables::HiraganaSymbolsTableFamily.Data()  // hiragana QCM
            : SymbolsTables::KatakanaSymbolsTableFamily.Data(); // katakana QCM

    for (SymbolsTableSection& SymbolSection : symbolsSections )
        for(Symbol& symbol : SymbolSection.Data())
            if (symbol.Enabled())
                entriesPool.push_back(&symbol);

    //************************ Initialize Random ************************
    std::vector<Symbol*> shuffledSymbols{};

    if (GetMy::GetInstance().AppSettingWidget().IsWeightedRandomEnabled())
    {
        // TODO Now learningState
    }
    else
    {
        FntSetting& fntSetting = GetMy::GetInstance().FntSettingWidget();
        Symbol* stem = *Tools::GetRandom(entriesPool.begin(), entriesPool.end());
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

        shuffledSymbols.reserve(entriesPool.size()-1);
        std::remove_copy(entriesPool.begin(), entriesPool.end(), std::back_inserter(shuffledSymbols), stem);
        std::shuffle(std::begin(shuffledSymbols), std::end(shuffledSymbols), Tools::rng_engine);
    }

    //************************ Clearing previous board ************************
    qDeleteAll(guesses);
    guesses.clear();

    //************************ Initialize UI stuff ************************
    int NbrOfEntriesLine = GetMy::GetInstance().AppSettingWidget().GetNumberOfEntryLine();
    int NbrOfEntriesRow = GetMy::GetInstance().AppSettingWidget().GetNumberOfEntryRow();
    int stemSlot = Tools::GetRandomInt(0, (NbrOfEntriesLine*NbrOfEntriesRow)-1);
    for(int i= 0; i<NbrOfEntriesLine*NbrOfEntriesRow; ++i)
    {
        div_t entryPos = div(i, NbrOfEntriesLine);
        QcmEntryGuess* foo = new QcmEntryGuess();
        guesses.append(foo);

        if (i == stemSlot)
            foo->SetGuess(stem, currentQcmType, true);
        else
            foo->SetGuess(shuffledSymbols[static_cast<std::vector<Symbol>::size_type>(i)], currentQcmType, false);

        ui->EntriesGridLayout->addWidget(foo, entryPos.rem, entryPos.quot);
    }

    for (QcmEntryGuess* entry : guesses)
        entry->CorrectFontSize();

    ui->ScoreCounter->setNum(scoreCounter);
    ui->ErrorsCounter->setNum(errorCounter);
}

void QcmExercice::OnGuessClicked(bool correct, QcmEntryGuess* entryGuess)
{
    if (correct)
    {
        ++scoreCounter;
        int appStatisticsScore = settingsSerializer->value("AppStatistics/score", 0).toInt();
        settingsSerializer->setValue("AppStatistics/score", ++appStatisticsScore);
        int learningState = entryGuess->GetSymbol()->LearningState();
        if (learningState < 5 )
            entryGuess->GetSymbol()->LearningState(learningState+1);
    }
    else
    {
        ++errorCounter;
        int appStatisticsError = settingsSerializer->value("AppStatistics/error", 0).toInt();
        settingsSerializer->setValue("AppStatistics/error", ++appStatisticsError);
        int learningState = entryGuess->GetSymbol()->LearningState();
        if (learningState > 0  )
            entryGuess->GetSymbol()->LearningState(learningState-1);
    }

    // TODO feedback and print previous answer in upper right counter ?
    InitializeExercice(currentQcmType);
}
