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
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_Kbd :
        {
            qcmSubType = QcmTypeEnum::KanaToRmj;
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingWidget().GetCurrentHiraganaFnt());
            ui->EntryGuess->setText(symbol->JP());
            break;
        }
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            qcmSubType = QcmTypeEnum::KanaToRmj;
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingWidget().GetCurrentKatakanaFnt());
            ui->EntryGuess->setText(symbol->JP());
            break;
        }
        case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_MCQ :
        case QcmExercice::QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            qcmSubType = QcmTypeEnum::RmjToKana;
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
    int newFontSize = (qcmSubType == QcmTypeEnum::RmjToKana)
                        ? GetMy::Instance().FntSettingWidget().GetAnswerRmjKanaSize()
                        : GetMy::Instance().FntSettingWidget().GetAnswerKanaRmjSize();
    QRect textRect = ui->EntryGuess->fontMetrics().boundingRect(ui->EntryGuess->text());
    QFont correctedFont = QFont(ui->EntryGuess->fontInfo().family(), newFontSize);
    ui->EntryGuess->setFont(correctedFont);
    bool corrected = false;

    while ( (textRect.height() > ui->EntryGuess->height() || textRect.width() > ui->EntryGuess->width()) && newFontSize > 10)
    {
        newFontSize -= 5;
        QFont correctedFont = QFont(ui->EntryGuess->fontInfo().family(), newFontSize);
        ui->EntryGuess->setFont(correctedFont);

        textRect = ui->EntryGuess->fontMetrics().boundingRect(ui->EntryGuess->text());
        corrected = true;
    }
    if (corrected && !fntWarnDisplayed)
    {
        fntWarnDisplayed = true;

//QString debug;
//if (textRect.height() <= ui->EntryGuess->height())
//    debug += "Height too big +"
//            "(textRectHeight : "+QString::number(textRect.height())+
//            "; EntryHeight : "+QString::number(ui->EntryGuess->height())+")\n";
//if (textRect.width() <= ui->EntryGuess->width())
//    debug += "Height too big +"
//            "(textRectHeight : "+QString::number(textRect.height())+
//            "; EntryHeight : "+QString::number(ui->EntryGuess->height())+")\n";
//debug += "TEXT : "+ui->EntryGuess->text();
//Tools::GetInstance().DisplayPopup(debug, 0.5f);

        Tools::GetInstance().DisplayPopup(
                    "MCQ Answers (cf :Settings->Fonts) font size("+
                    QString::number((qcmSubType == QcmTypeEnum::RmjToKana)
                                    ? GetMy::Instance().FntSettingWidget().GetAnswerRmjKanaSize()
                                    : GetMy::Instance().FntSettingWidget().GetAnswerKanaRmjSize())+
                    ") is too big,\n"
                    "Resizing them to " + QString::number(newFontSize));
        if (qcmSubType == QcmTypeEnum::RmjToKana)
            GetMy::Instance().FntSettingWidget().SetAnswerRmjKanaSize(newFontSize);
        else
            GetMy::Instance().FntSettingWidget().SetAnswerKanaRmjSize(newFontSize);
    }
}

void QcmEntryGuess::on_EntryGuess_clicked()
{
    GetMy::Instance().QcmExerciceWidget().OnGuessClicked(correctGuess, this);
}

bool QcmEntryGuess::fntWarnDisplayed = false;
