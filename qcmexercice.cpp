#include "qcmexercice.h"
#include "ui_qcmexercice.h"
#include "qcmentryguess.h"
#include "symbolstables.h"
#include "fntsetting.h"
#include "tools.h"
#include "GetMy.h"
#include <algorithm>
#include "SettingsSerializer.h"
#include "appsettings.h"

#define ENTRY_PER_ROW 3

QcmExercice::QcmExercice(QWidget *parent) :
    QWidget(parent), ui(new Ui::QcmExercice), scoreCounter(0), errorCounter(0)
  , currentQcmType(QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM)
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
    FntSetting& fntSetting = GetMy::GetInstance().FntSettingWidget();

    if (newQcmRequested)
    {
        currentQcmType = qcmType;
        scoreCounter = 0;
        errorCounter = 0;
    }

    Symbol stem = *Tools::GetRandom(SymbolsTables::HIRAGANA_GOJUON.begin(), SymbolsTables::HIRAGANA_GOJUON.end());
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
            ui->GuessMe->setText(QString::fromStdString(stem.romanji));
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM :
        {
            QFont stemFont = fntSetting.GetCurrentHiraganaFnt();
            stemFont.setPixelSize(stemFont.pixelSize() + fntSetting.GetStemBoostSize());
            ui->GuessMe->setFont(stemFont);
            ui->GuessMe->setText(stem.jp);
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM :
        {
            QFont stemFont = fntSetting.GetCurrentKatakanaFnt();
            stemFont.setPixelSize(stemFont.pixelSize() + fntSetting.GetStemBoostSize());
            ui->GuessMe->setFont(stemFont);
            ui->GuessMe->setText(stem.jp);
            break;
        }
    }

    std::vector<Symbol> shuffledSymbols{};
    int NbrOfEntriesLine = GetMy::GetInstance().AppSettingWidget().GetNumberOfEntryLine();
    // TODO NOW do a very basic hirahana / katakana swap
    shuffledSymbols.reserve(SymbolsTables::HIRAGANA_GOJUON.size()-1);
    std::remove_copy(SymbolsTables::HIRAGANA_GOJUON.begin(), SymbolsTables::HIRAGANA_GOJUON.end(), std::back_inserter(shuffledSymbols), stem);
    std::shuffle(std::begin(shuffledSymbols), std::end(shuffledSymbols), Tools::rng_engine);

    qDeleteAll(guesses);
    guesses.clear();

    int stemSlot = Tools::GetRandomInt(0, (NbrOfEntriesLine*ENTRY_PER_ROW)-1);
    for(int i= 0; i<NbrOfEntriesLine*ENTRY_PER_ROW; ++i)
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

    ui->ScoreCounter->setNum(scoreCounter);
    ui->ErrorsCounter->setNum(errorCounter);
}

void QcmExercice::OnGuessClicked(bool correct)
{
    if (correct)
    {
        ++scoreCounter;
        int appStatisticsScore = SettingsSerializer::settings.value("AppStatistics/score", 0).toInt();
        SettingsSerializer::settings.setValue("AppStatistics/score", appStatisticsScore+scoreCounter);
    }
    else
    {
        ++errorCounter;
        int appStatisticsError = SettingsSerializer::settings.value("AppStatistics/error", 0).toInt();
        SettingsSerializer::settings.setValue("AppStatistics/error", appStatisticsError+errorCounter);
    }

    // TODO feedback and print previous answer in upper right counter ?
    InitializeExercice(currentQcmType);
}
