#include "qcmexercice.h"
#include "ui_qcmexercice.h"
#include "qcmentryguess.h"
#include <string>
#include "symbolstables.h"
#include "tools.h"
#include <algorithm>

#define ENTRY_SIZE_POOL 9
#define ENTRY_PER_LINE 3

QcmExercice::QcmExercice(QWidget *parent) :
    QWidget(parent), ui(new Ui::QcmExercice), scoreCounter(0), errorCounter(0)
{
    ui->setupUi(this);
}

QcmExercice::~QcmExercice()
{
    delete ui;
}

void QcmExercice::InitializeExercice()
{
    Symbol answer = *Tools::GetRandom(SymbolsTables::HIRAGANA_GOJUON.begin(), SymbolsTables::HIRAGANA_GOJUON.end());
    ui->GuessMe->setText(QString::fromStdString(answer.romanji));

    std::vector<Symbol> shuffledSymbols{};
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
        {
            foo->SetGuess(answer.jp, true); // answer is not removed from the shuffledSymbols possibly causing double guess !
            ui->EntriesGridLayout->addWidget(foo, entryPos.quot, entryPos.rem);
        }
        else
        {
            foo->SetGuess(shuffledSymbols[i].jp, false);
            ui->EntriesGridLayout->addWidget(foo, entryPos.quot, entryPos.rem);
        }
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

    // TODO feedback and print correct answer (if wrong or right) for a few second (can be tweaked)

    InitializeExercice();
}
