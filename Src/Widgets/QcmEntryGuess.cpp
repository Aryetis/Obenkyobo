#include "Src/Widgets/QcmEntryGuess.h"
#include "ui_qcmentryguess.h"
#include "Src/Pages/FntSettingsPage.h"
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

void QcmEntryGuess::SetGuess(Kana* s, QcmExercicePage::QcmExerciceType qcmType, bool b)
{
    symbol = s;

    switch (qcmType)
    {
        case QcmExercicePage::QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExercicePage::QcmExerciceType::Hiragana_to_Romanji_Kbd :
        {
            qcmSubType = QcmTypeEnum::KanaToRmj;
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFnt());
            ui->EntryGuess->setText(symbol->JP());
            break;
        }
        case QcmExercicePage::QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExercicePage::QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            qcmSubType = QcmTypeEnum::KanaToRmj;
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFnt());
            ui->EntryGuess->setText(symbol->JP());
            break;
        }
        case QcmExercicePage::QcmExerciceType::Romanji_to_Hiragana_MCQ :
        case QcmExercicePage::QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            qcmSubType = QcmTypeEnum::RmjToKana;
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingsPageInst().GetCurrentRomanjiFnt());
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
                        ? GetMy::Instance().FntSettingsPageInst().GetAnswerRmjKanaSize()
                        : GetMy::Instance().FntSettingsPageInst().GetAnswerKanaRmjSize();
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
                                    ? GetMy::Instance().FntSettingsPageInst().GetAnswerRmjKanaSize()
                                    : GetMy::Instance().FntSettingsPageInst().GetAnswerKanaRmjSize())+
                    ") is too big,\n"
                    "Resizing them to " + QString::number(newFontSize));
        if (qcmSubType == QcmTypeEnum::RmjToKana)
            GetMy::Instance().FntSettingsPageInst().SetAnswerRmjKanaSize(newFontSize);
        else
            GetMy::Instance().FntSettingsPageInst().SetAnswerKanaRmjSize(newFontSize);
    }
}

void QcmEntryGuess::on_EntryGuess_clicked()
{
    GetMy::Instance().QcmExercicePageInst().OnGuessClicked(correctGuess, this);
}

bool QcmEntryGuess::fntWarnDisplayed = false;
