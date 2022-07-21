#include "Src/Widgets/QcmEntryGuess.h"
#include "ui_qcmentryguess.h"
#include "Src/Pages/FntSettingsPage.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"
#include "Src/Pages/QcmExercicePage.h"

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

void QcmEntryGuess::SetGuess(QcmDataEntry* symbol_, QcmExerciceType qcmType, bool displayKanji, std::optional<bool> correct /*= std::nullopt*/)
{
    symbol = symbol_;

    switch (qcmType)
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        {
            qcmSubType = QcmTypeEnum::KanaToRmj;
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFnt());
            ui->EntryGuess->setText(*symbol->Kanas());
            break;
        }
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            qcmSubType = QcmTypeEnum::KanaToRmj;
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFnt());
            ui->EntryGuess->setText(*symbol->Kanas());
            break;
        }
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            qcmSubType = QcmTypeEnum::RmjToKana;
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingsPageInst().GetCurrentRomanjiFnt());
            ui->EntryGuess->setText(*symbol->Romanji());
            break;
        }
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        {
            qcmSubType = QcmTypeEnum::RmjToKana; // TODO NOW rename this (stemIsRomanji, stemIsJP)
            ui->EntryGuess->setFont(GetMy::Instance().FntSettingsPageInst().GetCurrentRomanjiFnt());
            ui->EntryGuess->setText(*symbol->Romanji());
            break;
        }
        case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
        {
            qcmSubType = QcmTypeEnum::RmjToKana;

            if (displayKanji)
            {
                ui->EntryGuess->setFont(GetMy::Instance().FntSettingsPageInst().GetCurrentKanjiFnt());
                ui->EntryGuess->setText(*symbol->Kanjis());
            }
            else if (static_cast<VocabDataEntry*>(symbol)->GetFontType() == KanaFamilyEnum::hiragana)
            {
                ui->EntryGuess->setFont(GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFnt());
                ui->EntryGuess->setText(*symbol->Kanas());
            }
            else
            {
                ui->EntryGuess->setFont(GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFnt());
                ui->EntryGuess->setText(*symbol->Kanas());
            }
            break;
        }
    }

    if (correct.has_value())
        correctGuess = correct.value();
}

void QcmEntryGuess::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    CorrectFontSize();
}


void QcmEntryGuess::CorrectFontSize() // TODO NOW : move it to Tools and make it context agnostic
{
    int newFontSize = (qcmSubType == QcmTypeEnum::RmjToKana)
                        ? GetMy::Instance().FntSettingsPageInst().GetKanasAnswerRmjKanaSize()
                        : GetMy::Instance().FntSettingsPageInst().GetKanasAnswerKanaRmjSize();
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
//GetMy::Instance().ToolsInst()->DisplayPopup(debug, 0.5f);

        GetMy::Instance().ToolsInst()->DisplayPopup(
                    "MCQ Answers (cf :Settings->Fonts) font size("+
                    QString::number((qcmSubType == QcmTypeEnum::RmjToKana)
                                    ? GetMy::Instance().FntSettingsPageInst().GetKanasAnswerRmjKanaSize()
                                    : GetMy::Instance().FntSettingsPageInst().GetKanasAnswerKanaRmjSize())+
                    ") is too big,\n"
                    "Resizing them to " + QString::number(newFontSize));
        if (qcmSubType == QcmTypeEnum::RmjToKana)
            GetMy::Instance().FntSettingsPageInst().SetKanasAnswerRmjKanaSize(newFontSize);
        else
            GetMy::Instance().FntSettingsPageInst().SetKanasAnswerKanaRmjSize(newFontSize);
    }
}

void QcmEntryGuess::on_EntryGuess_clicked()
{
    GetMy::Instance().QcmExercicePageInst().OnGuessClicked(correctGuess, this);
}

bool QcmEntryGuess::fntWarnDisplayed = false;
