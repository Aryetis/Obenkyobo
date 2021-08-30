#include "qcmexercice.h"
#include "ui_qcmexercice.h"
#include "qcmentryguess.h"
#include <string>
#include "symbolstables.h"
#include "tools.h"

#define ENTRY_SIZE_POOL 9
#define ENTRY_PER_LINE 3

QcmExercice::QcmExercice(QWidget *parent) :
    QWidget(parent), ui(new Ui::QcmExercice)
{
    ui->setupUi(this);
}

void QcmExercice::InitializeExercice()
{
    std::string answer = *Tools::GetRandom(SymbolsTables::HIRAGANA_GOJUON.begin(), SymbolsTables::HIRAGANA_GOJUON.end());
    ui->GuessMe->setText(QString::fromStdString(answer));

    std::vector<std::string> shuffledSymbols = std::vector<std::string>(SymbolsTables::HIRAGANA_GOJUON);
    std::default_random_engine rng = std::default_random_engine{};
    std::shuffle(std::begin(shuffledSymbols), std::end(shuffledSymbols), rng);

    qDeleteAll(guesses);

    int answerSlot = Tools::GetRandomInt(0, ENTRY_SIZE_POOL);
    for(int i= 0; i<ENTRY_SIZE_POOL; ++i)
    {
        div_t entryPos = div(i, ENTRY_PER_LINE);
        QcmEntryGuess* foo = new QcmEntryGuess();
        guesses.append(foo);

        if (i == answerSlot)
        {
            foo->SetGuessText(answer);
            ui->EntriesGridLayout->addWidget(foo, entryPos.quot, entryPos.rem);
        }
        else
        {
            foo->SetGuessText(shuffledSymbols[i]);
            ui->EntriesGridLayout->addWidget(foo, entryPos.quot, entryPos.rem);
        }
    }

    ui->ScoreCounter->setNum(Tools::GetRandomInt());
    ui->ErrorsCounter->setNum(Tools::GetRandomInt());
}

QcmExercice::~QcmExercice()
{
    delete ui;
}
