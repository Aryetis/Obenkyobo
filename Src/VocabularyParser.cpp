#include <QFile>
#include <QTextStream>
#include "Src/VocabularyParser.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"

void VocabDataEntry::LearningState(int ls)
{
    vocabDataFile->WriteLearningScore(GetPath(), ls, lineNumber);
}

bool VocabDataEntry::IsEnabled() const
{
    // TODO NOW : shouldn't be pure virtual ?
    return true;
}

void VocabDataEntry::Enabled(bool /*b*/)
{
    // TODO NOW : shouldn't be pure virtual ?
    return;
}

VocabDataFile::VocabDataFile(QString sheetPath) : vocabSheetPath(sheetPath), entries(), malformedLines(), learningScore(0)
{
    QFile vocabFile(vocabSheetPath);
    if (vocabFile.open(QIODevice::ReadOnly))
    {
        int lineNbr = 0;
        QTextStream in(&vocabFile);
        in.setCodec("UTF-8");

        while (!in.atEnd())
            ParseLine(in.readLine(), lineNbr++);

        learningScore /= entries.count();
        vocabFile.close();
    }
    else
        GetMy::Instance().ToolsInst()->DisplayPopup("Could not open file : "+vocabSheetPath);
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
    KanaFamilyEnum fontType_ = KanaFamilyEnum::hiragana;

    if ( parsedFields.count() == 6 )
    {
        if (parsedFields[1] == "hiragana")
            fontType_ = KanaFamilyEnum::hiragana;
        else if (parsedFields[1] == "katakana")
            fontType_ = KanaFamilyEnum::katakana;
        else
        {
            malformedLines.insert(new VocabDataEntry(kanas_, kanji_, trad_, learningScore_, this, lineNumber_, fontType_));
            return;
        }

        kanas_ = parsedFields[2];
        kanji_ = parsedFields[3];
        trad_ = parsedFields[4];
        learningScore_ = parsedFields[5].toInt();
        if (learningScore_ < 0 || learningScore_ > MAX_LEARNING_STATE_VALUE)
        {
            malformedLines.insert(new VocabDataEntry(kanas_, kanji_, trad_, learningScore_, this, lineNumber_, fontType_));
            return;
        }
        else
            learningScore += learningScore_;

        entries.insert(new VocabDataEntry(kanas_, kanji_, trad_, learningScore_, this, lineNumber_, fontType_));
        return;
    }
    else
        malformedLines.insert(new VocabDataEntry(kanas_, kanji_, trad_, learningScore_, this, lineNumber_, fontType_));
}

bool VocabDataFile::WriteLearningScore(QString vocabSheetPath, int ls, int lineNumber /*= -1*/)
{
    QFile vocabFileIn{vocabSheetPath};
    if (QFile::exists(vocabSheetPath+".tmp"))
        QFile::remove(vocabSheetPath+".tmp");
    QFile vocabFileOut{vocabSheetPath+".tmp"};
    if (vocabFileIn.open(QIODevice::ReadOnly) && vocabFileOut.open(QIODevice::ReadWrite))
    {
        QTextStream in{&vocabFileIn};
        in.setCodec("UTF-8");
        QTextStream out{&vocabFileOut};
        out.setCodec("UTF-8");
        int lineCounter = 0;

        while (!in.atEnd())
        {
            QString curLine = in.readLine();
            if ((lineNumber == -1 || lineNumber == lineCounter) && (curLine.count() > 0 && curLine[0] != '#'))
            {
                curLine.replace(QRegularExpression("\\[learningScore=([0-9]+)\\]"), QString("[learningScore=%1]").arg(ls));
                out << curLine << "\n";
            }
            else
                out << curLine << "\n";

            ++lineCounter;
        }
        QString inName = vocabFileIn.fileName();
        vocabFileIn.close();
        vocabFileIn.remove();
        vocabFileOut.close();
        vocabFileOut.rename(inName);
        return true;
    }
    else
        GetMy::Instance().ToolsInst()->DisplayPopup("Could not open file : "+vocabSheetPath);

    return false;
}

bool VocabDataFile::ResetLearningScore(QString vocabSheetPath)
{
    // Reminder : LearningScore value is its weight in the qcm's pool <=> it's inversed
    return WriteLearningScore(vocabSheetPath, MAX_LEARNING_STATE_VALUE);
}

VocabDataPool::VocabDataPool(QString sheetPath)
{
    PopulateFromPath(sheetPath);
}

VocabDataPool::VocabDataPool(QSet<QString> sheetPaths)
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
