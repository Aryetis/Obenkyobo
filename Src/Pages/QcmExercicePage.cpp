#include <algorithm>
#include <QString>
#include "Src/Pages/QcmExercicePage.h"
#include "ui_QcmExercicePage.h"
#include "Src/Widgets/QcmEntryGuess.h"
#include "Src/Pages/FntSettingsPage.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/mainwindow.h"
#include "Src/KanasTables.h"
#include "Src/DefinesLand.h"

#include <QSizePolicy>

QcmExercicePage::QcmExercicePage(QWidget *parent) :
    QWidget(parent), ui(new Ui::QcmExercicePage), scoreCounter(0), errorCounter(0),
    currentQcmType(),
    refreshCounter(0), curHiraganaNonSized(), curKatakanaNonSized(), curRomanjiNonSized(), stemFont(),
    settingsSerializer(GetMy::Instance().SettingSerializerInst()),
    stemDisplayKanji(false), entriesPool({}), vdp(nullptr)
{
    ui->setupUi(this);

    ui->ResultLabelGroupBox->setFlat(true);
    ui->ResultLabelGroupBox->setStyleSheet("border : none; ");

    GetMy::Instance().SetQcmExercicePageInst(this);
}

QcmExercicePage::~QcmExercicePage()
{
    delete vdp;
    qDeleteAll(guesses);
    guesses.clear();
    delete ui;
}

bool QcmExercicePage::InitializeExercice(QcmExerciceType qcmType, bool newQcmRequested /* = false*/)
{
    FntSettingsPage& fntSetting = GetMy::Instance().FntSettingsPageInst();
    int NbrOfEntriesLine = GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryLine();
    int NbrOfEntriesRow = GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryRow();
    bool newQcpTypeRequested = (currentQcmType.has_value() && currentQcmType.value() != qcmType);
    currentQcmType = qcmType;

    //************************ BEGINNING OF NEW QCM REQUESTED ************************
    if (newQcmRequested)
    {
        //************************ "Is there enough to form the pool" ************************
        switch (qcmType)
        {
            case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
            case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
            {
                // Can't check if there's enough data right now. will be done as soon
                // as the VocabDataPool has been initialized
                break;
            }
            default:
            {
                if (!GetMy::Instance().ToolsInst()->IsThereEnough(qcmType))
                    return false;
                break;
            }
        }

        //********** Initialize Fonts, reset counters, displaySwitchButton **********
        scoreCounter = 0;
        errorCounter = 0;

        int labelRightPointSize = ui->ResultLabelRight->font().pointSize();
        curHiraganaNonSized = QFont(fntSetting.GetCurrentHiraganaFamily(), labelRightPointSize);
        curKatakanaNonSized = QFont(fntSetting.GetCurrentKatakanaFamily(), labelRightPointSize);
        curRomanjiNonSized = QFont(fntSetting.GetCurrentRomanjiFamily(), labelRightPointSize);
        curKanjiNonSized = QFont(fntSetting.GetCurrentKanjiFamily(), labelRightPointSize);

        switch (qcmType)
        {
            case QcmExerciceType::Hiragana_to_Romanji_MCQ :
            case QcmExerciceType::Hiragana_to_Romanji_Kbd :
            case QcmExerciceType::Katakana_to_Romanji_MCQ :
            case QcmExerciceType::Katakana_to_Romanji_Kbd :
            case QcmExerciceType::Romanji_to_Hiragana_MCQ :
            case QcmExerciceType::Romanji_to_Katakana_MCQ :
            {
                ui->SwitchButton->setVisible(false);
                break;
            }
            case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
            case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
            {
                ui->SwitchButton->setVisible(true);
                break;
            }
        }

        //************************ Initialize Entries Pool ************************
        entriesPool.clear();
        switch (qcmType)
        {
            case QcmExerciceType::Hiragana_to_Romanji_MCQ :
            case QcmExerciceType::Hiragana_to_Romanji_Kbd :
            case QcmExerciceType::Romanji_to_Hiragana_MCQ :
            {
                for (SymbolsTableSection& SymbolSection : KanasTables::HiraganaSymbolsTableFamily.Data() )
                    for(QcmDataEntryKana& symbol : SymbolSection.Data())
                        if (symbol.IsEnabled())
                            entriesPool.push_back(&symbol);
                break;
            }
            case QcmExerciceType::Katakana_to_Romanji_MCQ :
            case QcmExerciceType::Katakana_to_Romanji_Kbd :
            case QcmExerciceType::Romanji_to_Katakana_MCQ :
            {
                for (SymbolsTableSection& SymbolSection : KanasTables::KatakanaSymbolsTableFamily.Data() )
                    for(QcmDataEntryKana& symbol : SymbolSection.Data())
                        if (symbol.IsEnabled())
                            entriesPool.push_back(&symbol);
                break;
            }
            case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
            case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
            {
                if (vdp != nullptr)
                    delete vdp;
                vdp = new VocabDataPool{GetMy::Instance().GetEnabledVocabSheets()};
                if (!GetMy::Instance().ToolsInst()->IsThereEnough(qcmType, vdp->AllEntries().size()))
                    return false;
                entriesPool.insert(entriesPool.end(), vdp->AllEntries().begin(), vdp->AllEntries().end());
                break;
            }
        }
        //************ Clearing previous board and making new entries **************
        int nbrOfGuesses = NbrOfEntriesLine*NbrOfEntriesRow;
        if (guesses.count() > nbrOfGuesses)
        {
            for (int i=guesses.count()-1; i >= nbrOfGuesses ; --i)
            {
                ui->EntriesGridLayout->removeWidget(guesses[i]);
                delete guesses[i];
                guesses.removeLast();
            }
        }
        else if (guesses.count() < nbrOfGuesses)
        {
            for(int i=guesses.count(); i < NbrOfEntriesLine*NbrOfEntriesRow; ++i)
            {
                div_t entryPos = div(i, NbrOfEntriesRow);

                QcmEntryGuess* foo = new QcmEntryGuess(this);
                guesses.append(foo);
                ui->EntriesGridLayout->addWidget(foo, entryPos.quot, entryPos.rem);
            }
        }
    }
    //************************ END OF NEW QCM REQUESTED ************************

    //************************ Initialize Shuffled Symbols Pool ************************
    std::vector<QcmDataEntry*> shuffledSymbols{};
    shuffledSymbols.reserve(entriesPool.size()-1);
    std::remove_copy(entriesPool.begin(), entriesPool.end(), std::back_inserter(shuffledSymbols), stem);
    std::shuffle(std::begin(shuffledSymbols), std::end(shuffledSymbols), GetMy::Instance().ToolsInst()->Rng_Engine());

    //************************ Initialize Stem  ************************
    if (GetMy::Instance().AppSettingsPageInst().IsWeightedRandomEnabled())
    {
        std::vector<int> weights;
        for (QcmDataEntry* symbol : shuffledSymbols)
            weights.push_back(symbol->LearningScore());

        std::discrete_distribution<size_t> distr(weights.begin(), weights.end());
        stem = shuffledSymbols[distr(GetMy::Instance().ToolsInst()->MT())];
    }
    else
        stem = *GetMy::Instance().ToolsInst()->GetRandom(shuffledSymbols.begin(), shuffledSymbols.end());

    assert(stem != nullptr);

    //*************** Getting correct "base" stemFont  *****************
    InitializeStemFont();

    //************** Setting Stem's text (not applying) ****************
    switch (qcmType)
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
        {
            ui->Stem->setText(*stem->Romanji());
            break;
        }
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        {
            ui->Stem->setText((stemDisplayKanji) ? *stem->Kanjis() : *stem->Kanas());
            break;
        }
    }

    //************************ Initialize Entries board ************************
    int stemSlot = GetMy::Instance().ToolsInst()->GetRandomInt(0, (NbrOfEntriesLine*NbrOfEntriesRow)-1);
    QcmDataEntry* joker = shuffledSymbols[static_cast<std::vector<QcmDataEntry>::size_type>(stemSlot)]; // Symbol replaced by stem
    for(int i= 0; i<NbrOfEntriesLine*NbrOfEntriesRow; ++i)
    {
        div_t entryPos = div(i, NbrOfEntriesRow);

        QcmEntryGuess* foo = static_cast<QcmEntryGuess*>(ui->EntriesGridLayout->itemAtPosition(entryPos.quot, entryPos.rem)->widget());
        QcmDataEntry* curSym = shuffledSymbols[static_cast<std::vector<QcmDataEntry>::size_type>(i)];

        if (i == stemSlot)
            foo->SetGuessData(stem, currentQcmType.value(), stemDisplayKanji, true);
        else
        {
            if (curSym == stem) // avoid double entries
                foo->SetGuessData(joker, currentQcmType.value(), stemDisplayKanji, false);
            else
                foo->SetGuessData(curSym, currentQcmType.value(), stemDisplayKanji, false);
        }
    }

    //************************ UI score error Counters ************************
    ui->ScoreCounter->setNum(scoreCounter);
    ui->ErrorsCounter->setNum(errorCounter);

    //************************ Fonts Size Corrections ************************
    if (!newQcmRequested || newQcpTypeRequested) // first call needs to be delayed so everything/geometry is set up correctly
    {
        ApplyCorrectStemFontSize();
        ApplyGuessesTextAndCorrection();
    }

    //************************ Hard Refresh ************************
    int HardRefreshFreq = GetMy::Instance().AppSettingsPageInst().GetHardRefreshFreq();
    if (HardRefreshFreq > 0 && ++refreshCounter >= HardRefreshFreq)
    {
        refreshCounter = 0;
        GetMy::Instance().MainWindowInst().AggressiveClearScreen(true);
    }

    return true;
}

void QcmExercicePage::OnGuessClicked(bool correct, QcmEntryGuess* entryGuess)
{
    std::cout << "LOG : QcmExercice::OnGuessClicked()" << std::endl;
    if ( correct )
    {
        ++scoreCounter;
        int appStatisticsScore = settingsSerializer->value("AppStatistics/score", 0).toInt();
        settingsSerializer->setValue("AppStatistics/score", ++appStatisticsScore);
        int EntryGuessLearningState = entryGuess->GetSymbol()->LearningScore();
        if ( EntryGuessLearningState > 0 )
            entryGuess->GetSymbol()->LearningScore(EntryGuessLearningState-1);
    }
    else
    {
        ++errorCounter;
        int appStatisticsError = settingsSerializer->value("AppStatistics/error", 0).toInt();
        settingsSerializer->setValue("AppStatistics/error", ++appStatisticsError);
        int EntryGuessLearningState = entryGuess->GetSymbol()->LearningScore();
        if ( EntryGuessLearningState < MAX_LEARNING_STATE_VALUE )
            entryGuess->GetSymbol()->LearningScore(EntryGuessLearningState+1);
        if ( stem->LearningScore() < MAX_LEARNING_STATE_VALUE )
            stem->LearningScore(stem->LearningScore()+1);
    }


    // print feedback informing of correct / incorrect choice
    // TODO use the correct font for the jp part
    ui->ResultLabelLeft->setStyleSheet("QLabel { border: none }");
    ui->ResultLabelMiddle->setStyleSheet("QLabel { border: none }");
    switch (currentQcmType.value())
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        {
            ui->ResultLabelLeft->setFont(curRomanjiNonSized);
            ui->ResultLabelLeft->setText(*stem->Romanji());

            ui->ResultLabelMiddle->setFont(curHiraganaNonSized);
            ui->ResultLabelMiddle->setText(*stem->Kanas());
            break;
        }
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            ui->ResultLabelLeft->setFont(curRomanjiNonSized);
            ui->ResultLabelLeft->setText(*stem->Romanji());

            ui->ResultLabelMiddle->setFont(curKatakanaNonSized);
            ui->ResultLabelMiddle->setText(*stem->Kanas());
            break;
        }
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        {
            ui->ResultLabelLeft->setFont(curHiraganaNonSized);
            ui->ResultLabelLeft->setText(*stem->Kanas());

            ui->ResultLabelMiddle->setFont(curRomanjiNonSized);
            ui->ResultLabelMiddle->setText(*stem->Romanji());
            break;
        }
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            ui->ResultLabelLeft->setFont(curKatakanaNonSized);
            ui->ResultLabelLeft->setText(*stem->Kanas());

            ui->ResultLabelMiddle->setFont(curRomanjiNonSized);
            ui->ResultLabelMiddle->setText(*stem->Romanji());
            break;
        }
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        {
            if (stemDisplayKanji)
            {
                ui->ResultLabelLeft->setFont(curKanjiNonSized);
                ui->ResultLabelLeft->setText(*stem->Kanjis());
            }
            else if (static_cast<VocabDataEntry*>(stem)->GetFontType() == KanaFamilyEnum::hiragana)
            {
                ui->ResultLabelLeft->setFont(curHiraganaNonSized);
                ui->ResultLabelLeft->setText(*stem->Kanas());
            }
            else
            {
                ui->ResultLabelLeft->setFont(curKatakanaNonSized);
                ui->ResultLabelLeft->setText(*stem->Kanas());
            }

            ui->ResultLabelMiddle->setFont(curRomanjiNonSized);
            ui->ResultLabelMiddle->setText(*stem->Romanji());
            break;
        }
        case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
        {
            if (stemDisplayKanji)
            {
                ui->ResultLabelMiddle->setFont(curKanjiNonSized);
                ui->ResultLabelMiddle->setText(*stem->Kanjis());
            }
            else if (static_cast<VocabDataEntry*>(stem)->GetFontType() == KanaFamilyEnum::hiragana)
            {
                ui->ResultLabelMiddle->setFont(curHiraganaNonSized);
                ui->ResultLabelMiddle->setText(*stem->Kanas());
            }
            else
            {
                ui->ResultLabelMiddle->setFont(curKatakanaNonSized);
                ui->ResultLabelMiddle->setText(*stem->Kanas());
            }

            ui->ResultLabelLeft->setFont(curRomanjiNonSized);
            ui->ResultLabelLeft->setText(*stem->Romanji());
            break;
        }
    }

    ui->ResultLabelRight->setText((correct) ? "☑" : "☒");
    ui->ResultLabelRight->setStyleSheet("QLabel { border: none }");
    ui->ResultLabelGroupBox->setStyleSheet((correct) ? "QGroupBox { border : none }"
                                                     : "QGroupBox { border : 5px solid black }");

    InitializeExercice(currentQcmType.value());
}

void QcmExercicePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

int contentRectWidth = ui->PlayLayout->contentsRect().width();
int contentRectheight = ui->PlayLayout->contentsRect().height();

// NOTES : all margins, spacing, etc are set to 0 in .ui file EXCEPT FOR THE ROOT LAYOUT
//this->layout()->margin() != 0; => ADAPT FOLLOWING SIZE ACCORDING TO THIS OR SET IT TO 0 AND BUILD THE MARGIN INTO IT

//************************ Setting Score Layout Size ************************
ui->ScoreCounter->setFixedHeight(contentRectheight*0.075f);
ui->ScoreCounter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
ui->ScoreText->setFixedHeight(contentRectheight*0.075f);
ui->ScoreText->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

ui->ErrorsCounter->setFixedHeight(contentRectheight*0.075f);
ui->ErrorsCounter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
ui->ErrorsText->setFixedHeight(contentRectheight*0.075f);
ui->ErrorsText->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

ui->SwitchButton->setVisible(stemDisplayKanji);
ui->SwitchButton->setFixedHeight(contentRectheight*0.070f); // hacky margin
ui->SwitchButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
ui->ResultLabelGroupBox->setFixedHeight(contentRectheight*0.075f);
ui->ResultLabelGroupBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

//************************ Setting stem Size ************************
int stemHeight = contentRectheight / (GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryLine()+1);
ui->Stem->setFixedSize(contentRectWidth, stemHeight);
ui->Stem->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    ApplyCorrectStemFontSize();
    ApplyGuessesTextAndCorrection(); // Also setFixedSize for guesses
}

void QcmExercicePage::ApplyGuessesTextAndCorrection()
{
    int NbrOfEntriesLine = GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryLine();
    int NbrOfEntriesRow = GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryRow();

    int contentGridWidth = ui->PlayLayout->contentsRect().width();
    int contentGridHeight = ui->PlayLayout->contentsRect().height();

    int guessWidth = contentGridWidth / (NbrOfEntriesRow);
    int guessHeight = contentGridHeight / (NbrOfEntriesLine+1);

    for(QcmEntryGuess* guess : guesses)
        guess->ApplyGuessTextAndCorrection(guessWidth, guessHeight); // Guess Fnt Resize Popup handled on QcmEntryGuess's side
}

void QcmExercicePage::on_SwitchButton_clicked() // "Switch Kana"
{
    stemDisplayKanji = !stemDisplayKanji;
    ui->SwitchButton->setText(stemDisplayKanji ? "Switch to Kanas" : "Switch to Kanjis");

    if (currentQcmType == QcmExerciceType::Vocabulary_to_Romanji_MCQ)
    {
        for(QcmEntryGuess* guess : guesses)
        {
            // TODO MG : set correct size, don't recycle old one (kanji's /kana's)
            guess->SetGuessData(guess->GetSymbol(), currentQcmType.value(), stemDisplayKanji);
        }
        ApplyGuessesTextAndCorrection();
    }
    else
    {
        // TODO MG : set correct size, don't recycle old one (kanji's /kana's)
        ui->Stem->setText((stemDisplayKanji) ? *stem->Kanjis() : *stem->Kanas());
        ApplyCorrectStemFontSize();
    }
}

void QcmExercicePage::InitializeStemFont()
{
    FntSettingsPage& fntSetting = GetMy::Instance().FntSettingsPageInst();

    switch (currentQcmType.value())
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        {
            stemFont = QFont(fntSetting.GetCurrentRomanjiFamily(), fntSetting.GetKanasStemSize());
            break;
        }
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        {
            stemFont = QFont(fntSetting.GetCurrentHiraganaFamily(), fntSetting.GetKanasStemSize());
            break;
        }
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            stemFont = QFont(fntSetting.GetCurrentKatakanaFamily(), fntSetting.GetKanasStemSize());
            break;
        }
        case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
        {
            stemFont = QFont(fntSetting.GetCurrentRomanjiFamily(), fntSetting.GetVocabStemSize());
            break;
        }
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        {
            if (stemDisplayKanji)
                stemFont = QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentKanjiFamily(),
                                 GetMy::Instance().FntSettingsPageInst().GetVocabStemSize());
            else
            {
                if(static_cast<VocabDataEntry*>(stem)->GetFontType() == KanaFamilyEnum::hiragana)
                    stemFont = QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFamily(),
                                     GetMy::Instance().FntSettingsPageInst().GetVocabStemSize());
                else
                    stemFont = QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFamily(),
                                     GetMy::Instance().FntSettingsPageInst().GetVocabStemSize());
            }
            break;
        }
    }
}

void QcmExercicePage::ApplyCorrectStemFontSize()
{
    if (Tools::CorrectFontSize(ui->Stem->text(), ui->Stem->font(), *(ui->Stem), stemFont))
        ++continuousStemFntResizeCoRunter;
    else
        continuousStemFntResizeCoRunter = 0;
    ui->Stem->setFont(stemFont);

    // Stem Fnt Resize Popup
    if (continuousStemFntResizeCoRunter > POPUP_FNT_RESIZE_ERROR_CNT)
    {
        continuousStemFntResizeCoRunter = 0;

        int originalSize;
        int newSize = ui->Stem->font().pointSize();
        switch (currentQcmType.value())
        {
            case QcmExerciceType::Hiragana_to_Romanji_MCQ :
            case QcmExerciceType::Hiragana_to_Romanji_Kbd :
            case QcmExerciceType::Katakana_to_Romanji_MCQ :
            case QcmExerciceType::Katakana_to_Romanji_Kbd :
            case QcmExerciceType::Romanji_to_Hiragana_MCQ :
            case QcmExerciceType::Romanji_to_Katakana_MCQ :
            {
                originalSize = GetMy::Instance().FntSettingsPageInst().GetKanasStemSize();
                GetMy::Instance().FntSettingsPageInst().SetKanasStemSize(newSize);
                break;
            }
            case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
            case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
            {
                originalSize = GetMy::Instance().FntSettingsPageInst().GetVocabStemSize();
                GetMy::Instance().FntSettingsPageInst().SetVocabStemSize(newSize);
                break;
            }
        }

        GetMy::Instance().ToolsInst()->DisplayPopup(
                "Stem size ("+QString::number(originalSize)+") seems too big (cf :Settings->Fonts),\n"
                "Changing it to " + QString::number(newSize));
    }
}

int QcmExercicePage::continuousStemFntResizeCoRunter = 0;
