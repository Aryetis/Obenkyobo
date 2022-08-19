#include "Src/Widgets/QcmEntryGuess.h"
#include "ui_qcmentryguess.h"
#include "Src/Pages/FntSettingsPage.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"
#include "Src/Pages/QcmExercicePage.h"
// Notes to self :
// Order of execution : x*SetGuess() -> x*resizeEvent() -> x*paintEvent
// TODO : if you want wordWrap => rewrite everything using QLabel instead of QPushButton, have fun, fuck qt

QcmEntryGuess::QcmEntryGuess(QWidget *parent) :
    QWidget(parent),
    symbol(nullptr),
    ui(new Ui::QcmEntryGuess),
    sizeCorrected(false),
    spawning(true)
{
    ui->setupUi(this);
}

QcmEntryGuess::~QcmEntryGuess()
{
    delete ui;
}

void QcmEntryGuess::SetGuess(QcmDataEntry* symbol_, QcmExerciceType qcmType_, bool displayKanji_, std::optional<bool> correct /*= std::nullopt*/)
{
    // WARNING : DO NOT set text and font at this stage as it will expand QcmEntryGuess's layout =>
    // might result in expanding too much/squishing neighboor entries => inconsistent entries size

    symbol = symbol_;
    qcmType = qcmType_;
    if (correct.has_value())
        correctGuess = correct.value();
    displayKanji = displayKanji_;

    // recompute size everytime we set next guess OR switch to Kanji/Kanas <=> everytime we're not spawning
    if (!spawning)
    {
        // reset everything size related so we can have proper ComputeSizeCorrection()
        // because despites being told otherwise, qt does not enforce col/row stretch sometimes... fuck qt
        ui->EntryGuess->setText("");
        this->resize(vanillaWidth.value(), vanillaHeight.value()); // should trigger ComputeSizeCorrection() and repaint()
        // TODO NOW : Tools::CorrectFontSize could use vanillaWidth and vanillaHeight instead of this->geometry infos
        //              it would save a useless resize

        ComputeSizeCorrection();
        repaint();
    }
}

void QcmEntryGuess::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (spawning)
    {
        if (!vanillaHeight.has_value() || !vanillaWidth.has_value())
        {
            // Keeping in memory a proper size before any sizeCorrection shenanigans
            vanillaHeight = geometry().height();
            vanillaWidth = geometry().width();
        }
        ComputeSizeCorrection();
    }
}

void QcmEntryGuess::ComputeSizeCorrection()
{
    switch (qcmType)
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        {
            originalFntSize = GetMy::Instance().FntSettingsPageInst().GetKanasAnswerKanaRmjSize();
            sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                {GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFamily(), originalFntSize},
                                *(ui->EntryGuess), correctedFnt);
            break;
        }
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            originalFntSize = GetMy::Instance().FntSettingsPageInst().GetKanasAnswerKanaRmjSize();
            sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                {GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFamily(), originalFntSize},
                                *(ui->EntryGuess), correctedFnt);
            break;
        }
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            originalFntSize = GetMy::Instance().FntSettingsPageInst().GetKanasAnswerRmjKanaSize();
            sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                {GetMy::Instance().FntSettingsPageInst().GetCurrentRomanjiFamily(), originalFntSize},
                                *(ui->EntryGuess), correctedFnt);
            break;
        }
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        {
            originalFntSize = GetMy::Instance().FntSettingsPageInst().GetVocabAnswerRmjKanaSize();
            sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                {GetMy::Instance().FntSettingsPageInst().GetCurrentRomanjiFamily(), originalFntSize},
                                *(ui->EntryGuess), correctedFnt);
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
            }
            else if (static_cast<VocabDataEntry*>(symbol)->GetFontType() == KanaFamilyEnum::hiragana)
            {
                originalFntSize = GetMy::Instance().FntSettingsPageInst().GetVocabAnswerKanaRmjSize();
                sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                    {GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFamily(), originalFntSize},
                                    *(ui->EntryGuess), correctedFnt);
            }
            else
            {
                originalFntSize = GetMy::Instance().FntSettingsPageInst().GetVocabAnswerKanaRmjSize();
                sizeCorrected = Tools::CorrectFontSize(*symbol->Kanas(),
                                    {GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFamily(), originalFntSize},
                                    *(ui->EntryGuess), correctedFnt);
            }
            break;
        }
    }

    if (sizeCorrected)
    {
        if (!fntWarnDisplayed) // TODO : rethink UI design, shall I really set fnt setting or not
                               //       + popup will only be triggered once where it could be necessary to be displayed multiple times
                               //       eg : if kanji AND kanas sizes are both too big in a vocab QCM
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

void QcmEntryGuess::paintEvent(QPaintEvent *event)
{
    switch (qcmType)
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            ui->EntryGuess->setFont(correctedFnt);
            ui->EntryGuess->setText(*symbol->Kanas());
            break;
        }
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        {
            ui->EntryGuess->setFont(correctedFnt);
            ui->EntryGuess->setText(*symbol->Romanji());
            break;
        }
        case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
        {
            if (displayKanji)
            {
                ui->EntryGuess->setFont(correctedFnt);
                ui->EntryGuess->setText(*symbol->Kanjis());
            }
            else
            {
                ui->EntryGuess->setFont(correctedFnt);
                ui->EntryGuess->setText(*symbol->Kanas());
            }
            break;
        }
    }

    QWidget::paintEvent(event);

    spawning = false;
}

void QcmEntryGuess::on_EntryGuess_clicked()
{
    GetMy::Instance().QcmExercicePageInst().OnGuessClicked(correctGuess, this);
}

bool QcmEntryGuess::fntWarnDisplayed = false;
