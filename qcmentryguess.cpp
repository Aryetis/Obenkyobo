#include "qcmentryguess.h"
#include "ui_qcmentryguess.h"

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
