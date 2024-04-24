#include <algorithm>
#include <QSizePolicy>
#include "Src/Pages/VocabularyDisplayPage.h"
#include "ui_VocabularyDisplayPage.h"
#include "Src/Pages/FntSettingsPage.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/mainwindow.h"
#include "Src/Widgets/VocabBaseEntryWidget.h"
#include "Src/VocabularyParser.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"

VocabularyDisplayPage::VocabularyDisplayPage(QWidget *parent) :
    QWidget(parent), vdf(nullptr), ui(new Ui::VocabularyDisplayPage)
{
    ui->setupUi(this);

    ui->vocabGrid->setColumnStretch(0, 34);
    ui->vocabGrid->setColumnStretch(1, 24);
    ui->vocabGrid->setColumnStretch(2, 34);
    ui->vocabGrid->setColumnStretch(3, 8);

    ui->previousPageButton->setShortcut(KoboKey::Key_PagePackward);
    ui->nextPageButton->setShortcut(KoboKey::Key_PageForward);

    GetMy::Instance().SetVocabularyDisplayPageInst(this);
}

VocabularyDisplayPage::~VocabularyDisplayPage()
{
    for(auto lineLabels : gridLabels)
        qDeleteAll(lineLabels);
    gridLabels.clear();
    if (vdf != nullptr)
        delete vdf;
    delete ui;
}

void VocabularyDisplayPage::InitializeGrid(VocabFileEntryWidget* vocab)
{
    /************************ Initializing stuff ************************/
//    CleanGrid(); // called by PopulateGrid()
    curPage = 0;
    maxPage = 0;
    kanasShow = true;
    kanjiShow = true;
    traductionShow = true;
    lsShow = true;

    /************************ Parsing Vocab File ************************/
    vdf = new VocabDataFile(vocab->VocabFileInfo().filePath());
    if (vdf->MalformedLines().size() > 0)
    {
        QString popupMsg = "Malformed Vocab sheet, errors on lines : ";
        for (VocabDataEntry const* entry : vdf->MalformedLines())
            popupMsg += QString::number(entry->GetLineNumber()) + ", " ;
        popupMsg.chop(2);
        popupMsg += '.';
        GetMy::Instance().ToolsInst()->DisplayPopup(popupMsg);
    }
    if (vdf->Entries().size() == 0)
        vdf->Entries().insert(new VocabDataEntry("EMPTY",".OBEN","FILE  :(", MAX_LEARNING_STATE_VALUE,  nullptr, -1, KanaFamilyEnum::hiragana));


    maxPage = std::ceil(static_cast<float>(vdf->Entries().count()) / GetMy::Instance().AppSettingsPageInst().GetNumberOfRowPerVocabPage());

    /************************ Popuplating VocabGrid ************************/
    PopulateGrid();
}

void VocabularyDisplayPage::CleanGrid()
{
    for (int i=0; i<ui->vocabGrid->rowCount(); ++i)
        for (int j=0; j<ui->vocabGrid->columnCount();++j)
        {
            QLayoutItem* item = ui->vocabGrid->itemAtPosition(i, j);
            if (item != nullptr)
            {
                // Don't delete, recycle them for next page
                // need to hide ALL the element of the grid in case the next page contains
                // less elements than the previous one (and removeItem does not hide it)
                item->widget()->hide();
                ui->vocabGrid->removeItem(item);
            }
        }
}

void VocabularyDisplayPage::PopulateGrid(bool random /*= false*/, int turnPage /*= 0*/)
{
    // Checking if we have actual stuff to do
    if (curPage+turnPage >= 0 && curPage+turnPage < maxPage)
        curPage += turnPage;
    else if (!random)
        // No need to repopuplate and thus refresh the hide status if we're not really changing page or randomizing
        return;

    // Cleaning previous stuff
    CleanGrid();

    // Sort / Randomize
    QList<VocabDataEntry*> vdfEntriesList = vdf->Entries().values();
    if (random)
    {
        std::shuffle(vdfEntriesList.begin(), vdfEntriesList.end(), GetMy::Instance().ToolsInst()->MT());
        curPage = 0;
    }
    else
        std::sort(vdfEntriesList.begin(), vdfEntriesList.end(),
                  [](VocabDataEntry* a, VocabDataEntry* b)
                    {return a->GetLineNumber() < b->GetLineNumber();});

    // Pages stuff
    int x = 0;
    int curGridLine = 0;
    int nbrOfRow = GetMy::Instance().AppSettingsPageInst().GetNumberOfRowPerVocabPage();
    int fntSize = GetMy::Instance().AppSettingsPageInst().GetVocabFntSize();
    ui->previousPageButton->setCheckable(curPage != 0);
    ui->nextPageButton->setCheckable(curPage != maxPage);

    // Populating the grid
    for (int curVDEIdx = nbrOfRow*curPage; curVDEIdx < vdf->Entries().count() && curVDEIdx < nbrOfRow*(curPage+1); ++curVDEIdx)
    {
        VocabDataEntry* curVDE = vdfEntriesList[curVDEIdx];
        if ( static_cast<int>(gridLabels.size()) <= x) // reuse previous QPushButton, delete in destructor
            gridLabels.emplace_back(std::vector<QPushButton*>({new QPushButton(), new QPushButton(), new QPushButton(), new QPushButton()}));

        gridLabels[x][0]->setText(*curVDE->Kanas());
        gridLabels[x][0]->setFont
        ({(curVDE->GetFontType() == KanaFamilyEnum::hiragana)
                ? GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFamily()
                : GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFamily(), fntSize}
        );

        gridLabels[x][1]->setText(*curVDE->Kanjis());
        gridLabels[x][1]->setFont({GetMy::Instance().FntSettingsPageInst().GetCurrentKanjiFamily(), fntSize});

        gridLabels[x][2]->setText(*curVDE->Romanji()); // Romanji <=> Trad
        gridLabels[x][2]->setFont({gridLabels[x][2]->font().family(), fntSize});

        gridLabels[x][3]->setText(QString::number(MAX_LEARNING_STATE_VALUE - curVDE->LearningScore()));
        gridLabels[x][3]->setFont({gridLabels[x][3]->font().family(), fntSize});

        for (int j=0; j<4; ++j)
        {
            ui->vocabGrid->addWidget(gridLabels[x][j], curGridLine, j);
            gridLabels[x][j]->show();
            gridLabels[x][j]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
            gridLabels[x][j]->setCheckable(true);
            gridLabels[x][j]->setStyleSheet((j != 3)
                ? "QPushButton{ Text-align:left; border : none; } QPushButton:checked{ color: rgba(0,0,0,0); border : none; } QPushButton:focus { outline : none; }"
                : "QPushButton{ border : none; } QPushButton:checked{ color: rgba(0,0,0,0); border : none; } QPushButton:focus { outline : none; }");
        }

        ++curGridLine; ++x;
    }

    // Apply show status to columns
    HideColumn(0, kanasShow);
    HideColumn(1, kanjiShow);
    HideColumn(2, traductionShow);
    HideColumn(3, lsShow);
}

void VocabularyDisplayPage::on_randomizeButton_clicked()
{
    PopulateGrid(true);
    GetMy::Instance().MainWindowInst().AggressiveClearScreen();
}

void VocabularyDisplayPage::on_nextPageButton_clicked()
{
    PopulateGrid(false, 1);
    GetMy::Instance().MainWindowInst().AggressiveClearScreen();
}

void VocabularyDisplayPage::on_previousPageButton_clicked()
{
    PopulateGrid(false, -1);
    GetMy::Instance().MainWindowInst().AggressiveClearScreen();
}

void VocabularyDisplayPage::on_KanasHidePushButton_clicked()
{
    kanasShow = !kanasShow;
    ui->KanasHidePushButton->setCheckable(kanasShow);
    HideColumn(0, kanasShow);
}

void VocabularyDisplayPage::on_KanjiHidePushButton_clicked()
{
    kanjiShow = !kanjiShow;
    ui->KanjiHidePushButton->setCheckable(kanjiShow);
    HideColumn(1, kanjiShow);
}

void VocabularyDisplayPage::on_TraductionHidePushButton_clicked()
{
    traductionShow = !traductionShow;
    ui->TraductionHidePushButton->setCheckable(traductionShow);
    HideColumn(2, traductionShow);
}

void VocabularyDisplayPage::on_LSHidePushButton_clicked()
{
    lsShow = !lsShow;
    ui->LSHidePushButton->setCheckable(lsShow);
    HideColumn(3, lsShow);
}

void VocabularyDisplayPage::HideColumn(int col, bool b)
{
    for (int i=0; i<ui->vocabGrid->rowCount(); ++i)
    {
        QLayoutItem* item = ui->vocabGrid->itemAtPosition(i, col);
        if (item != nullptr)
        {
            QPushButton* butt = static_cast<QPushButton*>(item->widget());
            if (b)
                butt->setChecked(false); // "visible"
            else
                butt->setChecked(true); // "invisible"
        }
    }
}

void VocabularyDisplayPage::on_pushButton_clicked()
{
    vdf->ResetLearningScore();
    for (std::vector<QPushButton*> gridLine : gridLabels)
        gridLine[3]->setText(QString::number(0));
}
