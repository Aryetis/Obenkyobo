#include "qcmexercice.h"
#include "ui_qcmexercice.h"
#include "qcmentryguess.h"
#include "fntsetting.h"
#include "tools.h"
#include "GetMy.h"
#include <algorithm>
#include "appsettings.h"

#define ENTRY_PER_ROW 3

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
    std::vector<SymbolsTables::SymbolsTableSection>& symbolsSections = (qcmType < Katakana_to_Romanji_QCM)
            ? SymbolsTables::HiraganaSymbolsTableFamily.Data()  // hiragana QCM
            : SymbolsTables::KatakanaSymbolsTableFamily.Data(); // katakana QCM

    for (SymbolsTables::SymbolsTableSection& SymbolSection : symbolsSections )
        for(Symbol& symbol : SymbolSection.Data())
            if (symbol.GetEnabled())
                entriesPool.push_back(&symbol);

    //************************ Initialize Random ************************
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
            ui->GuessMe->setText(QString::fromStdString(stem->Romanji()));
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

    std::vector<Symbol*> shuffledSymbols{};
    shuffledSymbols.reserve(entriesPool.size()-1);
    std::remove_copy(entriesPool.begin(), entriesPool.end(), std::back_inserter(shuffledSymbols), stem);
    std::shuffle(std::begin(shuffledSymbols), std::end(shuffledSymbols), Tools::rng_engine);

    qDeleteAll(guesses);
    guesses.clear();

    //************************ Initialize UI stuff ************************
    int NbrOfEntriesLine = GetMy::GetInstance().AppSettingWidget().GetNumberOfEntryLine();
    int stemSlot = Tools::GetRandomInt(0, (NbrOfEntriesLine*ENTRY_PER_ROW)-1);
    for(int i= 0; i<NbrOfEntriesLine*ENTRY_PER_ROW; ++i)
    {
        div_t entryPos = div(i, NbrOfEntriesLine);
        QcmEntryGuess* foo = new QcmEntryGuess();
        guesses.append(foo);

        if (i == stemSlot)
            foo->SetGuess(*stem, currentQcmType, true);
        else
            foo->SetGuess(*(shuffledSymbols[static_cast<std::vector<Symbol>::size_type>(i)]), currentQcmType, false);

        ui->EntriesGridLayout->addWidget(foo, entryPos.rem, entryPos.quot);
    }

    ui->ScoreCounter->setNum(scoreCounter);
    ui->ErrorsCounter->setNum(errorCounter);
}

void QcmExercice::OnGuessClicked(bool correct)
{
    if (correct)
    {
        ++scoreCounter;
        int appStatisticsScore = settingsSerializer->value("AppStatistics/score", 0).toInt();
        settingsSerializer->setValue("AppStatistics/score", ++appStatisticsScore);
    }
    else
    {
        ++errorCounter;
        int appStatisticsError = settingsSerializer->value("AppStatistics/error", 0).toInt();
        settingsSerializer->setValue("AppStatistics/error", ++appStatisticsError);
    }

    // TODO : increase stem learning state (have a "guessStreak" counter [0;5] in it or something alike
    // with 0 <=> unknown, [1;3] learning, [4;5] learned)
    // do we decreased stem learned state or both stem and guessed ?

    // TODO feedback and print previous answer in upper right counter ?
    InitializeExercice(currentQcmType);
}
