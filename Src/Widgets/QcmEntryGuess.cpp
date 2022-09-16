#include "Src/Widgets/QcmEntryGuess.h"
#include "ui_QcmEntryGuess.h"
#include "Src/Pages/FntSettingsPage.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"
#include "Src/Pages/QcmExercicePage.h"
// Notes to self :
// Order of execution : x*SetGuess() -> x*resizeEvent() -> x*paintEvent

QcmEntryGuess::QcmEntryGuess(QWidget *parent) :
    QWidget(parent),
    symbol(nullptr),
    ui(new Ui::QcmEntryGuess),
    sizeCorrected(false)
{
    ui->setupUi(this);

    connect(this, SIGNAL(EntryGuessClickedSignal()), this, SLOT(On_EntryGuess_clicked()));
}

QcmEntryGuess::~QcmEntryGuess()
{
    delete ui;
}

void QcmEntryGuess::SetGuessData(QcmDataEntry* symbol_, QcmExerciceType qcmType_, bool displayKanji_, std::optional<bool> correct /*= std::nullopt*/)
{
    // WARNING : SetGuessData will NOT apply the text as it requires geometry information not necessarly available at time of the call
    symbol = symbol_;
    qcmType = qcmType_;
    if (correct.has_value())
        correctGuess = correct.value();
    displayKanji = displayKanji_;
}

bool QcmEntryGuess::ApplyGuessTextAndCorrection(int guessWidth, int guessHeight)
{    
    //************************ Set Fixed Size ************************
    ui->EntryGuess->setFixedSize(guessWidth, guessHeight);
    ui->EntryGuess->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //************************ Tools::CorrectFontSize, Apply corrected Fnt and Text ************************
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
            sizeCorrected = Tools::CorrectFontSize(*symbol->Romanji(),
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

    //************************ Guess Fnt Resize Popup ************************
    if (sizeCorrected)
    {
        ++continuousFntResizeCounter;

        if (continuousFntResizeCounter > POPUP_FNT_RESIZE_ERROR_CNT) // <=> "let it slides silently if only a few entries are too big"
        {
            continuousFntResizeCounter = 0;

            GetMy::Instance().ToolsInst()->DisplayPopup(
                    "MCQ Answers size ("+QString::number(originalFntSize)+") seems too big (cf :Settings->Fonts),\n"
                    "Changing it to " + QString::number(correctedFnt.pointSizeF()));
            switch (qcmType)
            {
                case QcmExerciceType::Hiragana_to_Romanji_MCQ :
                case QcmExerciceType::Hiragana_to_Romanji_Kbd :
                case QcmExerciceType::Katakana_to_Romanji_MCQ :
                case QcmExerciceType::Katakana_to_Romanji_Kbd :
                {
                    GetMy::Instance().FntSettingsPageInst().SetKanasAnswerKanaRmjSize(correctedFnt.pointSizeF());
                    break;
                }
                case QcmExerciceType::Romanji_to_Hiragana_MCQ :
                case QcmExerciceType::Romanji_to_Katakana_MCQ :
                {
                    GetMy::Instance().FntSettingsPageInst().SetKanasAnswerRmjKanaSize(correctedFnt.pointSizeF());
                    break;
                }
                case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
                {
                    GetMy::Instance().FntSettingsPageInst().SetVocabAnswerRmjKanaSize(correctedFnt.pointSizeF());
                    break;
                }
                case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
                {
                    GetMy::Instance().FntSettingsPageInst().SetVocabAnswerKanaRmjSize(correctedFnt.pointSizeF());
                    break;
                }
            }
        }
    }
    else
        continuousFntResizeCounter = 0;

    return sizeCorrected;
}

void QcmEntryGuess::mousePressEvent(QMouseEvent* /*event*/)
{
    emit EntryGuessClickedSignal();
}

void QcmEntryGuess::On_EntryGuess_clicked()
{
    GetMy::Instance().QcmExercicePageInst().OnGuessClicked(correctGuess, this);
}

int QcmEntryGuess::continuousFntResizeCounter = 0;
