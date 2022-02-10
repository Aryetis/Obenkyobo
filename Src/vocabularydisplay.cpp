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

    maxPage = std::ceil(gridEntries.count() / GetMy::Instance().AppSettingWidget().GetNbrOfRowPerVocabPage());

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
    /*************** Cleaning previous stuff, adding first Row ***************/
    CleanGrid();
    // TODO first row button stuff (should probably be done in QtDesigner

    /*************************** Rest of the grid ***************************/
    if (random)
    {
        std::shuffle(gridEntries.begin(), gridEntries.end(), Tools::GetInstance().MT());
        curPage = 0;
    }

    int curGridLine=0;
    int nbrOfRow = GetMy::Instance().AppSettingWidget().GetNbrOfRowPerVocabPage();
    if (curPage+turnPage >= 0 && curPage+turnPage < maxPage)
        curPage += turnPage;
    ui->previousPageButton->setCheckable(curPage != 0);
    ui->nextPageButton->setCheckable(curPage != maxPage);

    ui->vocabGrid->setColumnStretch(0, 40);
    ui->vocabGrid->setColumnStretch(1, 15);
    ui->vocabGrid->setColumnStretch(2, 40);
    ui->vocabGrid->setColumnStretch(3, 5);

    for (int i = nbrOfRow*curPage; i < gridEntries.count() && i < nbrOfRow*(curPage+1); ++i)
    {
        tempVocab* gridEntry = gridEntries[i];

        gridEntry->labels[0] = new QLabel(gridEntry->kanas);
        gridEntry->labels[0]->setFont
        ((gridEntry->fontType == SymbolFamilyEnum::hiragana) // TODO need to make its size similar to romanji one ?
                ? curHiraganaNonSized
                : curKatakanaNonSized
        );
        gridEntry->labels[1] = new QLabel(gridEntry->kanji); // TODO need a proper kanji font or test the default one (with/without jp dict installed)
        gridEntry->labels[2] = new QLabel(gridEntry->trad);
        gridEntry->labels[3] = new QLabel(QString::number(gridEntry->learningScore));
        gridEntry->labels[3]->setAlignment(Qt::AlignRight);

        for (int i=0; i<4; ++i)
            ui->vocabGrid->addWidget((gridEntry->labels[i]), curGridLine, i, Qt::AlignLeft );

        ++curGridLine;
    }
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
    for (int i=0; i<ui->vocabGrid->rowCount(); ++i)
    {
        QLayoutItem* item = ui->vocabGrid->itemAtPosition(i, 0);
        if (item != nullptr)
        {
            if (kanasShow)
                item->widget()->show();
            else
                item->widget()->hide();
        }
    }
}

void VocabularyDisplay::on_KanjiHidePushButton_clicked()
{
    kanjiShow = !kanjiShow;
    for (int i=0; i<ui->vocabGrid->rowCount(); ++i)
    {
        QLayoutItem* item = ui->vocabGrid->itemAtPosition(i, 1);
        if (item != nullptr)
        {
            if (kanjiShow)
                item->widget()->show();
            else
                item->widget()->hide();
        }
    }
}

void VocabularyDisplay::on_TraductionHidePushButton_clicked()
{
    traductionShow = !traductionShow;
    for (int i=0; i<ui->vocabGrid->rowCount(); ++i)
    {
        QLayoutItem* item = ui->vocabGrid->itemAtPosition(i, 2);
        if (item != nullptr)
        {
            if (traductionShow)
                item->widget()->show();
            else
                item->widget()->hide();
        }
    }
}

void VocabularyDisplay::on_LSHidePushButton_clicked()
{
    lsShow = !lsShow;
    for (int i=0; i<ui->vocabGrid->rowCount(); ++i)
    {
        QLayoutItem* item = ui->vocabGrid->itemAtPosition(i, 3);
        if (item != nullptr)
        {
            if (lsShow)
                item->widget()->show();
            else
                item->widget()->hide();
        }
    }
}
