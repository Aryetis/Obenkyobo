#include <QFile>
#include <QTextStream>
#include "Src/VocabularyParser.h"
#include "Src/tools.h"

void VocabDataEntry::SetLearningScore(int ls)
{
    // TODO later on, called during QCM
}

VocabDataFile::VocabDataFile(QString sheetPath) : vocabSheetPath(sheetPath), learningScore(0)
{
    QFile vocabFile(sheetPath);
    if (vocabFile.open(QIODevice::ReadOnly))
    {
        int lineNbr = 0;
        QTextStream in(&vocabFile);
        in.setCodec("UTF-8");

        while (!in.atEnd())
            ParseLine(in.readLine(), ++lineNbr);

        learningScore /= entries.count();
        vocabFile.close();
    }
    else
        Tools::GetInstance().DisplayPopup("Could not open file : "+sheetPath);
}

VocabDataFile::~VocabDataFile()
{
    qDeleteAll(entries);
    entries.clear();
    qDeleteAll(malformedLines);
    malformedLines.clear();
}

void VocabDataFile::ParseLine(const QString &line, int lineNumber_)
{
    if (line.count() <= 0 || line[0] == '#') // skip empty lines and comments
        return;

    //[fontType=hiragana][jp=sanity][kanji=kan][trad=check][learningScore=5]
    QRegExp rx("\\[fontType=([a-zA-Z]+)\\]\\[jp=([^\\]]+)\\]\\[kanji=([^\\]]+)\\]\\[trad=([^\\]]+)\\]\\[learningScore=([0-5])\\]");
    rx.indexIn(line);
    QStringList parsedFields = rx.capturedTexts(); // first one is matched line, not fields
    QString kanas_ = "";
    QString kanji_ = "";
    QString trad_ = "";
    int learningScore_ = -1;
    KanaFamilyEnum fontType_ = KanaFamilyEnum::dummy;

    if ( parsedFields.count() == 6 )
    {
        if (parsedFields[1] == "hiragana")
            fontType_ = KanaFamilyEnum::hiragana;
        else if (parsedFields[1] == "katakana")
            fontType_ = KanaFamilyEnum::katakana;
        else
            malformedLines.insert(new VocabDataEntry(kanas_, kanji_, trad_, learningScore_, this, lineNumber_, fontType_));

        kanas_ = parsedFields[2];
        kanji_ = parsedFields[3];
        trad_ = parsedFields[4];
        learningScore_ = parsedFields[5].toInt();
        if (learningScore_ < 0 || learningScore_ > MAX_LEARNING_STATE_VALUE)
            malformedLines.insert(new VocabDataEntry(kanas_, kanji_, trad_, learningScore_, this, lineNumber_, fontType_));
        else
            learningScore += learningScore_;

        entries.insert(new VocabDataEntry(kanas_, kanji_, trad_, learningScore_, this, lineNumber_, fontType_));
    }
    else
        malformedLines.insert(new VocabDataEntry(kanas_, kanji_, trad_, learningScore_, this, lineNumber_, fontType_));
}

VocabDataPool::VocabDataPool(QString sheetPath)
{
    PopulateFromPath(sheetPath);
}

VocabDataPool::VocabDataPool(QStringList sheetPaths)
{
    for(QString path : sheetPaths)
        PopulateFromPath(path);
}

VocabDataPool::~VocabDataPool()
{
    qDeleteAll(entries);
    entries.clear();
    qDeleteAll(malformedLines);
    malformedLines.clear();
    qDeleteAll(files);
    files.clear();
}

void VocabDataPool::PopulateFromPath(QString path)
{
    VocabDataFile* vdf = new VocabDataFile(path);
    files.insert(vdf);

    entries = entries.unite(vdf->Entries());
    malformedLines = malformedLines.unite(vdf->MalformedLines());
}
