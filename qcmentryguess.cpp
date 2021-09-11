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


void QcmEntryGuess::SetGuess(Symbol* s, QcmExercice::QcmExerciceType qcmType, bool b)
{
    symbol = s;

    switch (qcmType)
    {
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_Kbd :
        {
            ui->EntryGuess->setFont(GetMy::GetInstance().FntSettingWidget().GetCurrentHiraganaFnt());
            ui->EntryGuess->setText(symbol->JP());
            break;
        }
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            ui->EntryGuess->setFont(GetMy::GetInstance().FntSettingWidget().GetCurrentKatakanaFnt());
            ui->EntryGuess->setText(symbol->JP());
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM :
        case QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM :
        {
            ui->EntryGuess->setFont(GetMy::GetInstance().FntSettingWidget().GetCurrentRomanjiFnt());
            ui->EntryGuess->setText(symbol->Romanji());
            break;
        }
    }

    correctGuess = b;
}

void QcmEntryGuess::CorrectFontSize()
{
    int w = ui->EntryGuess->rect().width();
    int oldFontSize = ui->EntryGuess->font().pixelSize();
    int newFontSize = oldFontSize;
    QRect textRect = ui->EntryGuess->fontMetrics().boundingRect(ui->EntryGuess->text());
    while ( textRect.width() >  w ) // rect().h is incorrect, probably cause it get stretched afterwards.
                                    // TODO try to move this whole thing on QResizeEvent or similar ?
    {
        newFontSize -= 5;
        QFont correctedFont = QFont(ui->EntryGuess->font());
        correctedFont.setPixelSize(newFontSize);
        ui->EntryGuess->setFont(correctedFont);

        textRect = ui->EntryGuess->fontMetrics().boundingRect(ui->EntryGuess->text());
    }
}

void QcmEntryGuess::on_EntryGuess_clicked()
{
    GetMy::GetInstance().QcmExerciceWidget().OnGuessClicked(correctGuess, this);
}
