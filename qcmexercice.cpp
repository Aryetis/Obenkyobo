#include "qcmexercice.h"
#include "ui_qcmexercice.h"
#include "qcmentryguess.h"
#include "symbolstables.h"
#include "fntsetting.h"
#include "tools.h"
#include "GetMy.h"
#include <algorithm>

#define ENTRY_SIZE_POOL 9
#define ENTRY_PER_LINE 3

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

void QcmExercice::InitializeExercice(QcmExercice::QcmExerciceType qcmType)
{
    FntSetting& fntSetting = GetMy::GetInstance().FntSettingWidget();

    if (currentQcmType != qcmType)
    {
        currentQcmType = qcmType;
        scoreCounter = 0;
        errorCounter = 0;
    }

    Symbol answer = *Tools::GetRandom(SymbolsTables::HIRAGANA_GOJUON.begin(), SymbolsTables::HIRAGANA_GOJUON.end());
    switch (qcmType)
    {
        // TODO now fix font size
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_Kbd :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            ui->GuessMe->setFont(fntSetting.GetCurrentRomanjiFnt());
            ui->GuessMe->setText(QString::fromStdString(answer.romanji));
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM :
        {
            ui->GuessMe->setFont(fntSetting.GetCurrentHiraganaFnt());
            ui->GuessMe->setText(answer.jp);
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM :
        {
            ui->GuessMe->setFont(fntSetting.GetCurrentKatakanaFnt());
            ui->GuessMe->setText(answer.jp);
            break;
        }
    }

    std::vector<Symbol> shuffledSymbols{};
    // TODO NOW do a very basic hirahana / katakana swap
    shuffledSymbols.reserve(SymbolsTables::HIRAGANA_GOJUON.size()-1);
    std::remove_copy(SymbolsTables::HIRAGANA_GOJUON.begin(), SymbolsTables::HIRAGANA_GOJUON.end(), std::back_inserter(shuffledSymbols), answer);
    std::shuffle(std::begin(shuffledSymbols), std::end(shuffledSymbols), Tools::rng_engine);

    qDeleteAll(guesses);
    guesses.clear();

    int answerSlot = Tools::GetRandomInt(0, ENTRY_SIZE_POOL-1);
    for(int i= 0; i<ENTRY_SIZE_POOL; ++i)
    {
        div_t entryPos = div(i, ENTRY_PER_LINE);
        QcmEntryGuess* foo = new QcmEntryGuess();
        guesses.append(foo);

        if (i == answerSlot)
            foo->SetGuess(answer, currentQcmType, true);
        else
            foo->SetGuess(shuffledSymbols[static_cast<std::vector<Symbol>::size_type>(i)], currentQcmType, false);

        ui->EntriesGridLayout->addWidget(foo, entryPos.quot, entryPos.rem);
    }

    ui->ScoreCounter->setNum(scoreCounter);
    ui->ErrorsCounter->setNum(errorCounter);
}

void QcmExercice::OnGuessClicked(bool correct)
{
    if (correct)
        ++scoreCounter;
    else
        ++errorCounter;

    // TODO feedback and print previous answer in upper right counter ?
    InitializeExercice(currentQcmType);
}
