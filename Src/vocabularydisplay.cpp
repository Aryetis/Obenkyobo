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
    /************************ Cleaning previous stuff ************************/
    qDeleteAll(gridEntries);
    gridEntries.clear();

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
                QString jp_ = parsedFields[2];;
                QString kana_  = parsedFields[3];;
                QString trad_  = parsedFields[4];;
                int learningScore_ = parsedFields[5].toInt();;

                if (parsedFields[1] == "hiragana")
                    fontType_ = SymbolFamilyEnum::hiragana;
                else if (parsedFields[1] == "katakana")
                    fontType_ = SymbolFamilyEnum::katakana;
                else if (parsedFields[1] == "kanji")
                    fontType_ = SymbolFamilyEnum::kanji;
                else
                    continue;

                gridEntries.push_back(new tempVocab(fontType_, jp_, kana_, trad_, learningScore_));
            }
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
            QLabel(gridEntry->jp),
            QLabel(),
            QLabel(gridEntry->trad),
            QLabel(),
        };

        for (int i=0; i<4; ++i)
        {
            ui->vocabGrid->addWidget(&labels[i], curGridLine, i); // TODO not working ?
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

