#include "vocabularyentrydisplay.h"
#include "ui_vocabularyentrydisplay.h"

#include <QTextStream>
#include <tuple>
#include <optional>
#include <QLabel>

#include "symbolstables.h"

struct tempVocab
{
    std::optional<SymbolFamilyEnum> fontType;
    std::optional<QString> jp;
    std::optional<QString> trad;
    std::optional<int> learningScore;

    bool IsSane()
    {
        return fontType.has_value() && jp.has_value() && trad.has_value() && learningScore.has_value();
    }
};


VocabularyEntryDisplay::VocabularyEntryDisplay(VocabularyCfgListEntry vocabCfgEntry, QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyEntryDisplay)
{
    ui->setupUi(this);

    QFile vocabFile(vocabCfgEntry.VocabFileInfo().filePath());
    if (vocabFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&vocabFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (line.count() <= 0 || line[0] == '#') // skip empty lines and comments
            continue;

            QRegExp rx("(([^]+)=)(=([^]+))");
            QStringList resList = rx.capturedTexts();
            if (resList.count()%2 !=0) // malformed
                continue;

            tempVocab vocabStructEntry;
            int i =0;
            while (i*2 < resList.count())
            {
                if (resList[i] == "fontType")
                {
                    if (resList[i+1] == "hiragana")
                        vocabStructEntry.fontType = SymbolFamilyEnum::hiragana;
                    else if (resList[i+1] == "katakana")
                        vocabStructEntry.fontType = SymbolFamilyEnum::katakana;
                    else if (resList[i+1] == "kanji")
                        vocabStructEntry.fontType = SymbolFamilyEnum::kanji;
                }
                else if (resList[i] == "jp")
                    vocabStructEntry.jp = resList[i+1];
                else if (resList[i] == "trad")
                    vocabStructEntry.trad = resList[i+1];
                else if (resList[i] == "learningScore")
                    vocabStructEntry.learningScore = resList[i+1].toInt();

                i+=2;
            }

            if (vocabStructEntry.IsSane()) // TODO MG add to some "vocab entry set"
            {
                QHBoxLayout foo;
                QLabel jp(vocabStructEntry.jp.value()), trad(vocabStructEntry.trad.value());
                foo.addWidget(&jp);
                foo.addWidget(&trad);
                ui->scrollAreaWidgetContentsVLayout->addLayout(&foo);
            }
        }
        vocabFile.close();
    }
}

VocabularyEntryDisplay::VocabularyEntryDisplay(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyEntryDisplay)
{
    ui->setupUi(this);
}

VocabularyEntryDisplay::~VocabularyEntryDisplay()
{
    delete ui;
}

void VocabularyEntryDisplay::on_backButton_clicked()
{
//    GetMy::Instance().MainWindowWidget().on_actionLearn_Edit_Set_triggered()
}

void VocabularyEntryDisplay::on_hideButton_clicked()
{

}
