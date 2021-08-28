#include "qcmexercice.h"
#include "ui_qcmexercice.h"
#include "qcmentryguess.h"
#include <string>
#include "symbolstables.h"
#include "tools.h"

QcmExercice::QcmExercice(QWidget *parent) :
    QWidget(parent), ui(new Ui::QcmExercice)
{
    ui->setupUi(this);

    std::string answer = *Tools::GetRandom(SymbolsTables::HIRAGANA_GOJUON.begin(), SymbolsTables::HIRAGANA_GOJUON.end());
    ui->GuessMe->setText(QString::fromStdString(answer));

    QList<QcmEntryGuess*> entriesGuessList = ui->EntriesGridLayout->findChildren<QcmEntryGuess*>();

    int answerSlot = Tools::GetRandomInt(0, entriesGuessList.count());
    for(int i= 0; i<entriesGuessList.count(); ++i)
    {
        if (i == answerSlot)
        {
            entriesGuessList[i]->SetGuessText(answer);
        }
        else
        {
            std::string guessText = answer;
            while (guessText != answer)
            {
                guessText = *Tools::GetRandom(SymbolsTables::HIRAGANA_GOJUON.begin(), SymbolsTables::HIRAGANA_GOJUON.end());
            }
            entriesGuessList[i]->SetGuessText(guessText);
        }
    }
}

QcmExercice::~QcmExercice()
{
    delete ui;
}
