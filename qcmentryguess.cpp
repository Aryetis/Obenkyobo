#include "qcmentryguess.h"
#include "ui_qcmentryguess.h"
#include "fntsetting.h"
#include "GetMy.h"

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


void QcmEntryGuess::SetGuess(Symbol s, QcmExercice::QcmExerciceType qcmType, bool b)
{
    switch (qcmType)
    {
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_Kbd :
        {
            ui->EntryGuess->setFont(GetMy::GetInstance().FntSettingWidget().GetCurrentHiraganaFnt());
            ui->EntryGuess->setText(s.JP());
            break;
        }
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            ui->EntryGuess->setFont(GetMy::GetInstance().FntSettingWidget().GetCurrentKatakanaFnt());
            ui->EntryGuess->setText(s.JP());
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM :
        case QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM :
        {
            ui->EntryGuess->setFont(GetMy::GetInstance().FntSettingWidget().GetCurrentRomanjiFnt());
            ui->EntryGuess->setText(QString::fromStdString(s.Romanji()));
            break;
        }
    }

    correctGuess = b;
}


void QcmEntryGuess::on_EntryGuess_clicked()
{
    GetMy::GetInstance().QcmExerciceWidget().OnGuessClicked(correctGuess);
}
