#include "vocabularydisplay.h"
#include "ui_vocabularydisplay.h"
#include "Src/fntsetting.h"
#include "Src/tools.h"
#include "Src/appsettings.h"

#include <algorithm>
#include <QTextStream>

VocabularyDisplay::VocabularyDisplay(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyDisplay)
{
    ui->setupUi(this);

    curHiraganaNonSized = QFont(GetMy::Instance().FntSettingWidget().GetCurrentHiraganaFamily());
    curKatakanaNonSized = QFont(GetMy::Instance().FntSettingWidget().GetCurrentKatakanaFamily());

    ui->vocabGrid->setColumnStretch(0, 40);
    ui->vocabGrid->setColumnStretch(1, 15);
    ui->vocabGrid->setColumnStretch(2, 40);
    ui->vocabGrid->setColumnStretch(3, 5);

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
                QString kanas_ = parsedFields[2];;
                QString kanji_ = parsedFields[3];;
                QString trad_ = parsedFields[4];;
                int learningScore_ = parsedFields[5].toInt();;

                gridEntries.push_back(new tempVocab(fontType_, kanas_, kanji_, trad_, learningScore_));
            }
        }
        vocabFile.close();
    }

int tonpere = gridEntries.count();
int tamere = GetMy::Instance().AppSettingWidget().GetNbrOfRowPerVocabPage();
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
    if (curPage+turnPage >= 0 && curPage+turnPage < maxPage)
        curPage += turnPage;
    ui->previousPageButton->setCheckable(curPage != 0);
    ui->nextPageButton->setCheckable(curPage != maxPage);

    // Populating the grid
    for (int i = nbrOfRow*curPage; i < gridEntries.count() && i < nbrOfRow*(curPage+1); ++i)
    {
        tempVocab* gridEntry = gridEntries[i];

        // TODO : align romanji font to kanas and kanji one ?
        gridEntry->labels[0] = new QLabel(gridEntry->kanas);
        gridEntry->labels[0]->setFont
        ((gridEntry->fontType == SymbolFamilyEnum::hiragana)
                ? curHiraganaNonSized
                : curKatakanaNonSized
        );
        gridEntry->labels[1] = new QLabel(gridEntry->kanji);
        gridEntry->labels[1]->setFont(GetMy::Instance().FntSettingWidget().GetCurrentKanjiFnt());
        gridEntry->labels[2] = new QLabel(gridEntry->trad);
        gridEntry->labels[3] = new QLabel(QString::number(gridEntry->learningScore));
        gridEntry->labels[3]->setAlignment(Qt::AlignRight);

        for (int i=0; i<4; ++i)
            ui->vocabGrid->addWidget((gridEntry->labels[i]), curGridLine, i, Qt::AlignLeft );

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
}

void VocabularyDisplay::on_nextPageButton_clicked()
{
    PopulateGrid(false, 1);
}

void VocabularyDisplay::on_previousPageButton_clicked()
{
    PopulateGrid(false, -1);
}

void VocabularyDisplay::on_KanasHidePushButton_clicked()
{
    kanasShow = !kanasShow;
    HideColumn(0, kanasShow);
}

void VocabularyDisplay::on_KanjiHidePushButton_clicked()
{
    kanjiShow = !kanjiShow;
    HideColumn(1, kanjiShow);
}

void VocabularyDisplay::on_TraductionHidePushButton_clicked()
{
    traductionShow = !traductionShow;
    HideColumn(2, traductionShow);
}

void VocabularyDisplay::on_LSHidePushButton_clicked()
{
    lsShow = !lsShow;
    HideColumn(3, lsShow);
}

void VocabularyDisplay::HideColumn(int col, bool b)
{
    for (int i=0; i<ui->vocabGrid->rowCount(); ++i)
    {
        QLayoutItem* item = ui->vocabGrid->itemAtPosition(i, col);
        if (item != nullptr)
        {
            if (b)
                item->widget()->show();
            else
                item->widget()->hide();
        }
    }
}



