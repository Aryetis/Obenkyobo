#include "Src/Widgets/QcmEntryGuess.h"
#include "ui_qcmentryguess.h"
#include "Src/Pages/FntSettingsPage.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"
#include "Src/Pages/QcmExercicePage.h"
// Note to self :
// Order of execution : x*SetGuess() -> x*resizeEvent() -> x*paintEvent
// TODO : if you want wordWrap => rewrite everything using QLabel instead of QPushButton, have fun, fuck qt

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

void QcmEntryGuess::SetGuess(QcmDataEntry* symbol_, QcmExerciceType qcmType_, bool displayKanji, std::optional<bool> correct /*= std::nullopt*/)
{
    symbol = symbol_;
    qcmType = qcmType_;
    if (correct.has_value())
        correctGuess = correct.value();
    bool sizeCorrected = false;
    int originalFntSize;
    QFont correctedFnt;

    // WARNING : DO NOT set text and font at this stage as will expand QcmEntryGuess =>
    // might result in expanding too much/squishing neighboor entries => inconsistent entries size

    // TODO NOW : fix that shit
    //              => 1/ SET ENTRIES TO FIXED SIZE (at the very least width for now) !!!
    //                 2/ move size correction after resizeEvent determined the height's dimension
    //                 3/ enjoy ?
    switch (qcmType)
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        {
            originalFntSize = GetMy::Instance().FntSettingsPageInst().GetKanasAnswerKanaRmjSize();
            sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                {GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFamily(), originalFntSize},
                                *(ui->EntryGuess), correctedFnt);
            ui->EntryGuess->setFont(correctedFnt);
            ui->EntryGuess->setText(*symbol->Kanas());
            break;
        }
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            originalFntSize = GetMy::Instance().FntSettingsPageInst().GetKanasAnswerKanaRmjSize();
            sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                {GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFamily(), originalFntSize},
                                *(ui->EntryGuess), correctedFnt);
            ui->EntryGuess->setFont(correctedFnt);
            ui->EntryGuess->setText(*symbol->Kanas());
            break;
        }
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            originalFntSize = GetMy::Instance().FntSettingsPageInst().GetKanasAnswerRmjKanaSize();
            sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                {GetMy::Instance().FntSettingsPageInst().GetCurrentRomanjiFamily(), originalFntSize},
                                *(ui->EntryGuess), correctedFnt);
            ui->EntryGuess->setFont(correctedFnt);
            ui->EntryGuess->setText(*symbol->Romanji());
            break;
        }
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        {
            originalFntSize = GetMy::Instance().FntSettingsPageInst().GetVocabAnswerRmjKanaSize();
            sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                {GetMy::Instance().FntSettingsPageInst().GetCurrentRomanjiFamily(), originalFntSize},
                                *(ui->EntryGuess), correctedFnt);
            ui->EntryGuess->setFont(correctedFnt);
            ui->EntryGuess->setText(*symbol->Romanji());
            break;
        }
        case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
        {
            if (displayKanji)
            {
                originalFntSize = GetMy::Instance().FntSettingsPageInst().GetVocabAnswerKanaRmjSize();
                sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                    {GetMy::Instance().FntSettingsPageInst().GetCurrentKanjiFamily(), originalFntSize},
                                    *(ui->EntryGuess), correctedFnt);
                ui->EntryGuess->setFont(correctedFnt);
                ui->EntryGuess->setText(*symbol->Kanjis());
            }
            else if (static_cast<VocabDataEntry*>(symbol)->GetFontType() == KanaFamilyEnum::hiragana)
            {
                originalFntSize = GetMy::Instance().FntSettingsPageInst().GetVocabAnswerKanaRmjSize();
                sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                    {GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFamily(), originalFntSize},
                                    *(ui->EntryGuess), correctedFnt);
                ui->EntryGuess->setFont(correctedFnt);
                ui->EntryGuess->setText(*symbol->Kanas());
            }
            else
            {
                originalFntSize = GetMy::Instance().FntSettingsPageInst().GetVocabAnswerKanaRmjSize();
                sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                    {GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFamily(), originalFntSize},
                                    *(ui->EntryGuess), correctedFnt);
                ui->EntryGuess->setFont(correctedFnt);
                ui->EntryGuess->setText(*symbol->Kanas());
            }
            break;
        }
    }

    if (sizeCorrected)
    {
        if (!fntWarnDisplayed) // TODO : rethink design
        {
            GetMy::Instance().ToolsInst()->DisplayPopup(
                    "MCQ Answers (cf :Settings->Fonts) font size("+QString::number(originalFntSize)+") is too big,\n"
                    "Resizing them to " + QString::number(correctedFnt.pointSizeF()));
            fntWarnDisplayed = true;
        }
        switch (qcmType)
        {
            case Hiragana_to_Romanji_MCQ :
            case Katakana_to_Romanji_MCQ :
                {
//                    if (qcmSubType == QcmTypeEnum::RmjToKana)
//                        GetMy::Instance().FntSettingsPageInst().SetKanasAnswerRmjKanaSize(newFontSize);
//                    else
//                        GetMy::Instance().FntSettingsPageInst().SetKanasAnswerKanaRmjSize(newFontSize);
                    break;
                }
            case Romanji_to_Hiragana_MCQ :
            case Romanji_to_Katakana_MCQ :

            case Vocabulary_to_Romanji_MCQ :
            case Romanji_to_Vocabulary_MCQ :

            case Katakana_to_Romanji_Kbd :
            case Hiragana_to_Romanji_Kbd :
                { break; }
        }
    }
}

void QcmEntryGuess::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
}

void QcmEntryGuess::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void QcmEntryGuess::on_EntryGuess_clicked()
{
    GetMy::Instance().QcmExercicePageInst().OnGuessClicked(correctGuess, this);
}

bool QcmEntryGuess::fntWarnDisplayed = false;
