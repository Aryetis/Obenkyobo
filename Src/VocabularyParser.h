#ifndef VOCABULARYPARSER_H
#define VOCABULARYPARSER_H

#include "Src/KanasTables.h"

#include <QString>
#include <QSet>

class VocabDataEntry;
class VocabDataFile
{
    public :
        VocabDataFile() = default;
        VocabDataFile(QString path_);
        ~VocabDataFile();
        QString const& GetPath() const { return vocabSheetPath; }
        QSet<VocabDataEntry*>& Entries() { return entries; }
        QSet<VocabDataEntry*>& MalformedLines() { return malformedLines; }
        int GetLearningScore() const { return learningScore; }  // TODO : call this from VocabExplorerPage

    private :
        void ParseLine(QString const& line, int lineNumber);

        QString vocabSheetPath;
        QSet<VocabDataEntry*> entries;
        QSet<VocabDataEntry*> malformedLines; // nothing is guaranteed to be valid/parsed except LineNumber and VocabDataFile
        int learningScore;
};

class VocabDataEntry
{
    public :
        VocabDataEntry() = delete;
        VocabDataEntry(QString kanas_, QString kanji_, QString trad_, int ls_, VocabDataFile* vocabDataFile_, int lineNumber_, KanaFamilyEnum fontType_) :
            kanas(kanas_), kanji(kanji_), trad(trad_), learningScore(ls_), vocabDataFile(vocabDataFile_), lineNumber(lineNumber_), fontType(fontType_) {} // TODO : don't copy, move instead

        bool operator==(const VocabDataEntry& rhs) const
        {
            return kanas == rhs.kanas && kanji == rhs.kanji && trad == rhs.trad && learningScore == rhs.learningScore && GetPath() == rhs.GetPath() && lineNumber == rhs.lineNumber;
        }

        QString const& GetKanas() const { return kanas; }
        QString const& GetKanji() const { return kanji; }
        QString const& GetTrad() const { return trad; }
        int GetLearningScore() const { return learningScore; }
        void SetLearningScore(int ls); // TODO : Need to save ls into vocabSheetPath
        QString const& GetPath() const { return vocabDataFile->GetPath(); }
        int GetLineNumber() const { return lineNumber; }
        KanaFamilyEnum GetFontType() const { return fontType; }
//        bool IsSane() { return kanas != "" && kanji != "" && trad != "" && lineNumber != -1; }

    private :
        QString kanas;
        QString kanji;
        QString trad;
        int learningScore;
        VocabDataFile* vocabDataFile; // that way we don't store the same string xxx times for each file entry
        int lineNumber;
        KanaFamilyEnum fontType;
};
inline uint qHash(const VocabDataEntry &key, uint seed)
{
    return qHash(key.GetKanas(), seed) ^ qHash(key.GetKanji(), seed+1) ^ qHash(key.GetTrad(), seed+2) ^ qHash(key.GetPath(), seed+3);
}

class VocabDataPool
{
    public :
        VocabDataPool() = delete;
        VocabDataPool(QString sheetPath);
        VocabDataPool(QStringList sheetPaths);
        ~VocabDataPool();
        QSet<VocabDataEntry*>& AllEntries() { return entries; }

    private :
        void PopulateFromPath(QString path);
        bool DoesLineContain(VocabDataEntry vde);
        QSet<VocabDataEntry*> entries;
        QSet<VocabDataEntry*> malformedLines;
        QSet<VocabDataFile*> files;
};

#endif // VOCABULARYPARSER_H
