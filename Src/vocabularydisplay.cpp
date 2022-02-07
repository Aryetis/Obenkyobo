#include "vocabularydisplay.h"
#include "ui_vocabularydisplay.h"
#include "Src/fntsetting.h"

#include <QTextStream>

VocabularyDisplay::VocabularyDisplay(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyDisplay)
{
    ui->setupUi(this);

    curHiraganaNonSized = QFont(GetMy::Instance().FntSettingWidget().GetCurrentHiraganaFamily());
    curKatakanaNonSized = QFont(GetMy::Instance().FntSettingWidget().GetCurrentKatakanaFamily());

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

    for (tempVocab * gridEntry : gridEntries)
    {
        gridEntry->labels[0] = new QLabel(gridEntry->jp); // TODO data is garbage, check parsing with unicode or whatever
        gridEntry->labels[0]->setFont
        ((gridEntry->fontType == SymbolFamilyEnum::hiragana)
                ? curHiraganaNonSized
                : curKatakanaNonSized
        );
        gridEntry->labels[1] = new QLabel();
        gridEntry->labels[2] = new QLabel(gridEntry->trad);
        gridEntry->labels[3] = new QLabel();

        for (int i=0; i<4; ++i)
        {
            ui->vocabGrid->addWidget((gridEntry->labels[i]), curGridLine, i);
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

