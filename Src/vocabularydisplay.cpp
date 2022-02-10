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

        for (int i=0; i<4; ++i)
            ui->vocabGrid->addWidget((gridEntry->labels[i]), curGridLine, i);

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
