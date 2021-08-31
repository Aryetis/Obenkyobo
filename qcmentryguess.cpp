#include "qcmentryguess.h"
#include "ui_qcmentryguess.h"
#include "qcmexercice.h"

QcmEntryGuess::QcmEntryGuess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QcmEntryGuess)
{
    ui->setupUi(this);
}

QcmEntryGuess::~QcmEntryGuess()
{
    delete ui;
}


void QcmEntryGuess::SetGuess(QString s, bool b)
{
    ui->EntryGuess->setText(s);
    correctGuess = b;
}


void QcmEntryGuess::on_EntryGuess_clicked()
{
    QcmExercice::GetInstance().OnGuessClicked(correctGuess);
}
