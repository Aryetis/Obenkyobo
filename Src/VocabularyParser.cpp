#include <QFile>
#include <QTextStream>
#include "Src/VocabularyParser.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"



#include <regex>
#include <unistd.h>

void VocabDataEntry::LearningScore(int ls) // TODO MG ... this kinda loose its purpose ...
{
    vocabDataFileLnk->WriteLearningScore(GetPath(), {{ls, this}}); // will take care of updating learningScore through friendship
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
    rx.indexIn(line); // TODO Replace with pas du tout du
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

bool VocabDataFile::WriteLearningScore(QString vocabSheetPath, std::vector<std::pair<int, int>> transaction)
{   // transaction : <ls, lineNumber>, yes it's ugly, no I don't mind
    QFile vocabFile{vocabSheetPath};
    bool ret = true;
    if (vocabFile.open(QIODevice::ReadWrite))
    {
        /************** Read vocabSheetPath **************/
        QTextStream fileStream{&vocabFile};
        fileStream.setCodec("UTF-8");
        QStringList fileContent;
        while (!fileStream.atEnd())
            fileContent.append(fileStream.readLine());

        /************** Compute content **************/
        for (std::pair<int,int> const& trans : transaction)
        {   // <ls, lineNumber>
            if (trans.second>fileContent.size())
            {
                std::cerr << "Can't access line " << trans.second << " from file " << vocabSheetPath.toStdString().c_str() << std::endl;
                return false;
            }

            if (trans.second < 0) // rewrite every matching line
            {
                for (QString& curLine : fileContent)
                {
                    if (curLine.size() <= 0 || curLine[0] == '#')
                        continue;

                    QRegularExpression rx("\\[learningScore=([0-9]{1})\\]");
                    QRegularExpressionMatch match = rx.match(curLine);
                    if (match.capturedTexts().size() != 2)
                    {
                        std::cerr << "[WriteLearningScore] : incorrect number of [learningScore] group in " << vocabSheetPath.toStdString().c_str() << "'s line :" << curLine.toStdString().c_str() << std::endl;
                        ret = false;
                        continue;
                    }
                    else
                    {
                        int pos = match.capturedStart(1);
                        curLine[pos] = static_cast<char>('0'+trans.first);
                    }
                }
            }
            else // normal transaction, just rewrite corresponding lineNumber
            {
                QString targetLine = fileContent[trans.second]; // assuming lineNumber is pointing to a sane line
                if (targetLine.size() > 0 && targetLine[0] != '#')
                {
                    QRegularExpression rx("\\[learningScore=([0-9]{1})\\]");
                    QRegularExpressionMatch match = rx.match(targetLine);
                    QString& curLine = fileContent[trans.second];
                    if (match.capturedTexts().size() != 2)
                    {
                        std::cerr << "[WriteLearningScore] : incorrect number of [learningScore] group in " << vocabSheetPath.toStdString().c_str() << "'s line :" << curLine.toStdString().c_str() << std::endl;
                        ret = false;
                        continue;
                    }
                    else
                    {
                        int pos = match.capturedStart(1);
                        curLine[pos] = static_cast<char>('0'+trans.first);
                    }
                }
            }
        } // for transaction

        /************** Rewrite vocabSheetPath **************/
        vocabFile.resize(0);
        fileStream.reset();
        for (QString const& curLine : fileContent)
            fileStream << curLine << "\r\n";
        vocabFile.close();
    }
    else
    {
        std::cerr << "Could not open file" << std::endl;
        return false;
    }

    return ret;
}

bool VocabDataFile::WriteLearningScore(QString vocabSheetPath, std::vector<std::pair<int, VocabDataEntry*>> transaction)
{   // transaction : <ls, vde>, yes it's ugly, no I don't mind
    /****************** Updating File ******************/
    if ( !WriteLearningScore(vocabSheetPath, transaction) )
        return false;

    /****************** Updating Fields ******************/
    for (std::pair<int, VocabDataEntry*> const& trans : transaction)
    {
        if (trans.second != nullptr)
        {
            learningScore = learningScore - trans.second->LearningScore()/entries.size() + trans.first/entries.size();
            trans.second->learningScore = trans.first;
        }
        else
        {
            learningScore = trans.first;
            QSet<VocabDataEntry*>::iterator it = entries.begin();
            while (it != entries.end())
                (*it++)->learningScore = trans.first;
        }
    }

    return true;
}

bool VocabDataFile::ResetLearningScore(QString vocabSheetPath)
{
    return VocabDataFile::WriteLearningScore(vocabSheetPath, {{MAX_LEARNING_STATE_VALUE, -1}});
}

bool VocabDataFile::ResetLearningScore()
{
    // Reminder : LearningScore value is its weight in the qcm's pool <=> it's inversed
    return WriteLearningScore(vocabSheetPath, {{MAX_LEARNING_STATE_VALUE, nullptr}});
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
