#include "Src/VocabularyParser.h"
#include "Src/KanasTables.h"

#include <QFile>
#include <QTextStream>

void VocabDataEntry::SetLearningScore(int ls)
{

}

VocabDataFile::VocabDataFile(QString sheetPath) : vocabSheetPath(sheetPath)
{
    QFile vocabFile(sheetPath);
    if (vocabFile.open(QIODevice::ReadOnly))
    {
        int lineNbr = 0;
        QTextStream in(&vocabFile);
        in.setCodec("UTF-8");

        while (!in.atEnd())
            ParseLine(in.readLine(), ++lineNbr);

        vocabFile.close();
    }
}

void VocabDataFile::ParseLine(const QString &line, int lineNumber)
{
    if (line.count() <= 0 || line[0] == '#') // skip empty lines and comments
        return;

    //[fontType=hiragana][jp=sanity][kanji=kan][trad=check][learningScore=5]
    QRegExp rx("\\[fontType=([a-zA-Z]+)\\]\\[jp=([^\\]]+)\\]\\[kanji=([^\\]]+)\\]\\[trad=([^\\]]+)\\]\\[learningScore=([0-5])\\]");
    rx.indexIn(line);
    QStringList parsedFields = rx.capturedTexts(); // first one is matched line, not fields
    QString kanas = "";
    QString kanji = "";
    QString trad = "";
    int learningScore = -1;

    if ( parsedFields.count() == 6 )
    {
        if ( !(parsedFields[1] == "hiragana" || parsedFields[1] == "katakana") )
        {
            malformedLines.insert({kanas, kanji, trad, learningScore, this, lineNumber});
            return;
        }
        kanas = parsedFields[2];
        kanji = parsedFields[3];
        trad = parsedFields[4];
        learningScore = parsedFields[5].toInt();
        if (learningScore < 0 || learningScore > Kana::GetMaxlearningState())
        {
            malformedLines.insert({kanas, kanji, trad, learningScore, this, lineNumber});
            return;
        }

        entries.insert({kanas, kanji, trad, learningScore, this, lineNumber});
    }
    else
        malformedLines.insert({kanas, kanji, trad, learningScore, this, lineNumber});
}

VocabDataPool::VocabDataPool(QString sheetPath)
{

}

VocabDataPool::VocabDataPool(QStringList sheetPath)
{

}
