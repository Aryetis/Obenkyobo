#include <QFile>
#include <QTextStream>
#include "Src/VocabularyParser.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"

void VocabDataEntry::LearningScore(int ls)
{
    vocabDataFileLnk->WriteLearningScore(GetPath(), ls, this); // will take care of updating learningScore through friendship
}

QFont VocabDataEntry::GetFont(bool kanji)
{
    if (kanji)
        return QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentKanjiFamily(),
                     GetMy::Instance().FntSettingsPageInst().GetVocabStemSize());
    else if (fontType == KanaFamilyEnum::hiragana)
        return QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentHiraganaFamily(),
                     GetMy::Instance().FntSettingsPageInst().GetVocabStemSize());
    else
        return QFont(GetMy::Instance().FntSettingsPageInst().GetCurrentKatakanaFamily(),
                     GetMy::Instance().FntSettingsPageInst().GetVocabStemSize());
}

VocabDataFile::VocabDataFile(QString sheetPath, VocabDataPool* pool_) : vocabSheetPath(sheetPath), entries(), malformedLines(), poolLnks(),  learningScore(0)
{
    if (pool_ != nullptr)
        poolLnks.insert(pool_);

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
    for (VocabDataPool* pool : poolLnks)
        pool->RemoveVDF(this);
    poolLnks.clear();
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
    /****************** Updating File ******************/
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
    }
    else
    {
        GetMy::Instance().ToolsInst()->DisplayPopup("Could not open file : "+vocabSheetPath);
        return false;
    }

    return true;
}

bool VocabDataFile::WriteLearningScore(QString vocabSheetPath, int ls, VocabDataEntry* vde /*= nullptr*/)
{
    /****************** Updating File ******************/
    if ( !WriteLearningScore(vocabSheetPath, ls, (vde != nullptr) ? vde->GetLineNumber() : -1) )
        return false;

    /****************** Updating Fields ******************/
    if (vde != nullptr)
    {
        learningScore = learningScore - vde->LearningScore()/entries.size() + ls/entries.size();
        vde->learningScore = ls;
    }
    else
    {
        learningScore = ls;
        QSet<VocabDataEntry*>::iterator it = entries.begin();
        while (it != entries.end())
            (*it++)->learningScore = ls;
    }

    return true;
}

bool VocabDataFile::ResetLearningScore(QString vocabSheetPath)
{
    return VocabDataFile::WriteLearningScore(vocabSheetPath, MAX_LEARNING_STATE_VALUE, -1);
}

bool VocabDataFile::ResetLearningScore()
{
    // Reminder : LearningScore value is its weight in the qcm's pool <=> it's inversed
    return WriteLearningScore(vocabSheetPath, MAX_LEARNING_STATE_VALUE, nullptr);
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
    Clear();
}

void VocabDataPool::RemoveVDF(VocabDataFile* vdf)
{
    entriesLnks.subtract(vdf->Entries());
    malformedLinesLnks.subtract(vdf->Entries());
    files.remove(vdf);
}

void VocabDataPool::PopulateFromPath(QString path)
{
    VocabDataFile* vdf = new VocabDataFile(path, this);
    files.insert(vdf);

    entriesLnks.unite(vdf->Entries());
    malformedLinesLnks.unite(vdf->MalformedLines());
}

void VocabDataPool::PopulateFromPaths(QSet<QString> sheetPaths)
{
    for(QString path : sheetPaths)
        PopulateFromPath(path);
}

void VocabDataPool::Clear()
{
    if (!files.isEmpty())
    {
        QVector<VocabDataFile*> orphansVDF{}; // careful : ~VocabDataFile will unregister itself from pools, modifying their files in the process
                                              // can't safely iterate over files while deleting its content. thus this two steps deletion
        QSet<VocabDataFile*>::iterator it = files.begin();
        while (it != files.end())
        {
            if ((*it)->GetPoolLnks().size() == 1)
                orphansVDF.push_back(*it++);
            else
                // just because this pool is dead doesn't mean VDF isn't referenced by another pool (not happening for now but who knows)
                files.erase(it);
        }
        qDeleteAll(orphansVDF);

        files.clear();
    }

    entriesLnks.clear();
    malformedLinesLnks.clear();
}
