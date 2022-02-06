#include "vocabularydisplay.h"
#include "ui_vocabularydisplay.h"

#include <QTextStream>
#include <QLabel>

VocabularyDisplay::VocabularyDisplay(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyDisplay)
{
    ui->setupUi(this);
    GetMy::Instance().SetVocabularyDisplayWidget(this);
}

void VocabularyDisplay::InitializeGrid(VocabularyCfgListEntry* vocab)
{
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

            // TODO fuck this
            QRegExp rx("\[fontType=(?'fontType'[a-zA-Z]+)]\[jp=(?'jp'[^\]]+)]\[kanji=(?'kanji'[^\]]+)]\[trad=(?'trad'[^\]]+)]\[learningScore=(?'LS'[0-9])]");
            QStringList resList = rx.capturedTexts();
            if (resList.count()%4 !=0) // malformed
                continue;

            tempVocab* vocabStructEntry = new tempVocab();
            int i =0;
            while (i*2 < resList.count())
            {
                if (resList[i] == "fontType")
                {
                    if (resList[i+1] == "hiragana")
                        vocabStructEntry->fontType = SymbolFamilyEnum::hiragana;
                    else if (resList[i+1] == "katakana")
                        vocabStructEntry->fontType = SymbolFamilyEnum::katakana;
                    else if (resList[i+1] == "kanji")
                        vocabStructEntry->fontType = SymbolFamilyEnum::kanji;
                }
                else if (resList[i] == "jp")
                    vocabStructEntry->jp = resList[i+1];
                else if (resList[i] == "trad")
                    vocabStructEntry->trad = resList[i+1];
                else if (resList[i] == "learningScore")
                    vocabStructEntry->learningScore = resList[i+1].toInt();

                i+=2;
            }

            if (vocabStructEntry->IsSane())
                gridEntries.push_back(vocabStructEntry);
            else
                delete vocabStructEntry;
        }
        vocabFile.close();
    }

    /************************ Popuplating VocabGrid ************************/
    // First line

    // Rest of the grid
    int curGridLine=0;
    for (tempVocab const*const gridEntry : gridEntries)
    {
        QLabel labels[]
        {
            QLabel(gridEntry->jp.value()),
            QLabel(),
            QLabel(gridEntry->trad.value()),
            QLabel(),
        };

        for (int i=0; i<4; ++i)
        {
            ui->vocabGrid->addWidget(&labels[i], curGridLine, i);
        }

        ++curGridLine;
    }

}

VocabularyDisplay::~VocabularyDisplay()
{
    qDeleteAll(gridEntries);
    gridEntries.clear();
    delete ui;
}

