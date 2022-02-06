#include "Src/qcmentryguess.h"
#include "ui_qcmentryguess.h"
#include "Src/fntsetting.h"
#include "Src/GetMy.h"
#include "Src/tools.h"

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
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingWidget().GetCurrentHiraganaFnt());
            ui->EntryGuess->setText(symbol->JP());
            break;
        }
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingWidget().GetCurrentKatakanaFnt());
            ui->EntryGuess->setText(symbol->JP());
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM :
        case QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM :
        {
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingWidget().GetCurrentRomanjiFnt());
            ui->EntryGuess->setText(symbol->Romanji());
            break;
        }
    }

    correctGuess = b;
}

void QcmEntryGuess::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    CorrectFontSize();
}


void QcmEntryGuess::CorrectFontSize()
{
    int newFontSize = ui->EntryGuess->font().pixelSize();
    QRect textRect = ui->EntryGuess->fontMetrics().boundingRect(ui->EntryGuess->text());
    bool corrected = textRect.height() >  height() || textRect.width() > width();
    while ( textRect.height() >  height() || textRect.width() > width() )
    {
        newFontSize -= 10;
        QFont correctedFont = QFont(ui->EntryGuess->font());
        correctedFont.setPixelSize(newFontSize);
        ui->EntryGuess->setFont(correctedFont);

        textRect = ui->EntryGuess->fontMetrics().boundingRect(ui->EntryGuess->text());
    }
    if (corrected && !fntWarnDisplayed)
    {
        Tools::GetInstance().DisplayPopup(
                    "MCQ Entries font is too big,\nplease consider changing their size\n(Settings->Fonts). \n"
                    "Resizing them to " + QString::number(newFontSize) + " for now.");
        fntWarnDisplayed = true;
    }
}

void QcmEntryGuess::on_EntryGuess_clicked()
{
    GetMy::Instance().QcmExerciceWidget().OnGuessClicked(correctGuess, this);
}

bool QcmEntryGuess::fntWarnDisplayed = false;
