#include "vocabularydisplay.h"
#include "ui_vocabularydisplay.h"
#include "Src/fntsetting.h"
#include "Src/tools.h"
#include "Src/appsettings.h"
#include "Src/mainwindow.h"

#include <algorithm>
#include <QTextStream>
#include <QSizePolicy>
#include <QKeyEvent>
#include <QShortcut>

VocabularyDisplay::VocabularyDisplay(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyDisplay)
{
    ui->setupUi(this);

    ui->vocabGrid->setColumnStretch(0, 34);
    ui->vocabGrid->setColumnStretch(1, 24);
    ui->vocabGrid->setColumnStretch(2, 34);
    ui->vocabGrid->setColumnStretch(3, 8);

    ui->previousPageButton->setShortcut(KoboKey::Key_PagePackward);
    ui->nextPageButton->setShortcut(KoboKey::Key_PageForward);

    GetMy::Instance().SetVocabularyDisplayWidget(this);
}

VocabularyDisplay::~VocabularyDisplay()
{
    qDeleteAll(gridEntries);
    gridEntries.clear();
    delete ui;
}

void VocabularyDisplay::InitializeGrid(VocabularyCfgListEntry* vocab)
{
    /************************ Cleaning previous stuff ************************/
    CleanGrid();
    qDeleteAll(gridEntries);
    gridEntries.clear();
    curPage = 0;
    maxPage = 0;
    kanasShow = true;
    kanjiShow = true;
    traductionShow = true;
    lsShow = true;

    /************************ Parsing Vocab File ************************/
    QFile vocabFile(vocab->VocabFileInfo().filePath());
    if (vocabFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&vocabFile);
        in.setCodec("UTF-8");
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (line.count() <= 0 || line[0] == '#') // skip empty lines and comments
                continue;

            //[fontType=hiragana][jp=sanity][kanji=kan][trad=check][learningScore=5]
            QRegExp rx("\\[fontType=([a-zA-Z]+)\\]\\[jp=([^\\]]+)\\]\\[kanji=([^\\]]+)\\]\\[trad=([^\\]]+)\\]\\[learningScore=([0-5])\\]");
            rx.indexIn(line);
            QStringList parsedFields = rx.capturedTexts(); // first one is matched line, not fields

            if ( parsedFields.count() == 6 )
            {
                SymbolFamilyEnum fontType_;
                if (parsedFields[1] == "hiragana")
                    fontType_ = SymbolFamilyEnum::hiragana;
                else if (parsedFields[1] == "katakana")
                    fontType_ = SymbolFamilyEnum::katakana;
                else
                    continue;
                QString kanas_ = parsedFields[2];
                QString kanji_ = parsedFields[3];
                QString trad_ = parsedFields[4];
                int learningScore_ = parsedFields[5].toInt();

                gridEntries.push_back(new tempVocab(fontType_, kanas_, kanji_, trad_, learningScore_));
            }
        }
        vocabFile.close();
    }

    maxPage = std::ceil(static_cast<float>(gridEntries.count()) / GetMy::Instance().AppSettingWidget().GetNbrOfRowPerVocabPage());

    /************************ Popuplating VocabGrid ************************/
    PopulateGrid();
}

void VocabularyDisplay::CleanGrid()
{
    for (int i=0; i<ui->vocabGrid->rowCount(); ++i)
        for (int j=0; j<ui->vocabGrid->columnCount();++j)
        {
            QLayoutItem* item = ui->vocabGrid->itemAtPosition(i, j);
            if (item != nullptr)
                delete item->widget();
        }
}

void VocabularyDisplay::PopulateGrid(bool random /*= false*/, int turnPage /*= 0*/)
{
    // Cleaning previous stuff
    CleanGrid();

    // Randomize
    if (random)
    {
        std::shuffle(gridEntries.begin(), gridEntries.end(), Tools::GetInstance().MT());
        curPage = 0;
    }

    // Pages stuff
    int curGridLine=0;
    int nbrOfRow = GetMy::Instance().AppSettingWidget().GetNbrOfRowPerVocabPage();
    int fntSize = GetMy::Instance().AppSettingWidget().GetVocabFntSize();
    if (curPage+turnPage >= 0 && curPage+turnPage < maxPage)
        curPage += turnPage;
    ui->previousPageButton->setCheckable(curPage != 0);
    ui->nextPageButton->setCheckable(curPage != maxPage);

    // Populating the grid
    for (int i = nbrOfRow*curPage; i < gridEntries.count() && i < nbrOfRow*(curPage+1); ++i)
    {
        tempVocab* gridEntry = gridEntries[i];

        gridEntry->labels[0] = new QPushButton(gridEntry->kanas);
        gridEntry->labels[0]->setFont
        ({(gridEntry->fontType == SymbolFamilyEnum::hiragana)
                ? GetMy::Instance().FntSettingWidget().GetCurrentHiraganaFamily()
                : GetMy::Instance().FntSettingWidget().GetCurrentKatakanaFamily(), fntSize}
        );
        gridEntry->labels[1] = new QPushButton(gridEntry->kanji);
        gridEntry->labels[1]->setFont({GetMy::Instance().FntSettingWidget().GetCurrentKanjiFamily(), fntSize});
        gridEntry->labels[2] = new QPushButton(gridEntry->trad);
        gridEntry->labels[2]->setFont({gridEntry->labels[2]->font().family(), fntSize});
        gridEntry->labels[3] = new QPushButton(QString::number(gridEntry->learningScore));
        gridEntry->labels[3]->setFont({gridEntry->labels[3]->font().family(), fntSize});

        for (int j=0; j<4; ++j)
        {
            ui->vocabGrid->addWidget((gridEntry->labels[j]), curGridLine, j);
            gridEntry->labels[j]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
            gridEntry->labels[j]->setCheckable(true);
            gridEntry->labels[j]->setStyleSheet((j != 3)
                ? "QPushButton{ Text-align:left; border : none; } QPushButton:checked{ color: rgba(0,0,0,0); border : none; } QPushButton:focus { outline : none; }"
                : "QPushButton{ border : none; } QPushButton:checked{ color: rgba(0,0,0,0); border : none; } QPushButton:focus { outline : none; }");
        }

        ++curGridLine;
    }

    // Apply show status to columns
    HideColumn(0, kanasShow);
    HideColumn(1, kanjiShow);
    HideColumn(2, traductionShow);
    HideColumn(3, lsShow);
}

void VocabularyDisplay::on_randomizeButton_clicked()
{
    PopulateGrid(true);
    GetMy::Instance().MainWindowWidget().AggressiveClearScreen();
}

void VocabularyDisplay::on_nextPageButton_clicked()
{
    PopulateGrid(false, 1);
    GetMy::Instance().MainWindowWidget().AggressiveClearScreen();
}

void VocabularyDisplay::on_previousPageButton_clicked()
{
    PopulateGrid(false, -1);
    GetMy::Instance().MainWindowWidget().AggressiveClearScreen();
}

void VocabularyDisplay::on_KanasHidePushButton_clicked()
{
    kanasShow = !kanasShow;
    ui->KanasHidePushButton->setCheckable(kanasShow);
    HideColumn(0, kanasShow);
}

void VocabularyDisplay::on_KanjiHidePushButton_clicked()
{
    kanjiShow = !kanjiShow;
    ui->KanjiHidePushButton->setCheckable(kanjiShow);
    HideColumn(1, kanjiShow);
}

void VocabularyDisplay::on_TraductionHidePushButton_clicked()
{
    traductionShow = !traductionShow;
    ui->TraductionHidePushButton->setCheckable(traductionShow);
    HideColumn(2, traductionShow);
}

void VocabularyDisplay::on_LSHidePushButton_clicked()
{
    lsShow = !lsShow;
    ui->LSHidePushButton->setCheckable(lsShow);
    HideColumn(3, lsShow);
}

void VocabularyDisplay::HideColumn(int col, bool b)
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



