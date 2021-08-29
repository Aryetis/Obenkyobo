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

    int answerSlot = Tools::GetRandomInt(0, ENTRY_SIZE_POOL);
    for(int i= 0; i<ENTRY_SIZE_POOL; ++i)
    {
        div_t entryPos = div(i, ENTRY_PER_LINE);
        QcmEntryGuess* foo = new QcmEntryGuess();

        if (i == answerSlot)
        {
            ui->EntriesGridLayout->addWidget(foo, entryPos.quot, entryPos.rem);
            foo->SetGuessText(answer);
        }
        else
        {
            std::string guessText = answer;
            while (guessText == answer)
            {
                guessText = *Tools::GetRandom(SymbolsTables::HIRAGANA_GOJUON.begin(), SymbolsTables::HIRAGANA_GOJUON.end());
            }
            ui->EntriesGridLayout->addWidget(foo, entryPos.quot, entryPos.rem);
            foo->SetGuessText(answer);
        }
    }

    ui->ScoreCounter->setNum(Tools::GetRandomInt());
    ui->ErrorsCounter->setNum(Tools::GetRandomInt());
}

QcmExercice::~QcmExercice()
{
    delete ui;
}
