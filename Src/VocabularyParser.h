#ifndef VOCABULARYPARSER_H
#define VOCABULARYPARSER_H

#include <QString>
#include <QSet>

class VocabDataEntry;
class VocabDataFile
{
    public :
        VocabDataFile(QString path_);
        QString const& GetPath() { return vocabSheetPath; }

    private :
        void ParseLine(QString const& line, int lineNumber);

        QString vocabSheetPath;
        QSet<VocabDataEntry> entries;
        QSet<VocabDataEntry> malformedLines;
};

class VocabDataEntry
{
    public :
//        VocabDataEntry() : kanas(""), kanji(""), trad(""), learningScore(-1), vocabDataFile(nullptr), lineNumber(-1) {}
        VocabDataEntry(QString kanas_, QString kanji_, QString trad_, int ls_, VocabDataFile* vocabDataFile_, int lineNumber_) :
            kanas(kanas_), kanji(kanji_), trad(trad_), learningScore(ls_), vocabDataFile(vocabDataFile_), lineNumber(lineNumber_) {}

        bool operator==(const VocabDataEntry& rhs) const
        {
            return kanas == rhs.kanas && kanji == rhs.kanji && trad == rhs.trad && learningScore == rhs.learningScore && GetPath() == rhs.GetPath() && lineNumber == rhs.lineNumber;
        }

        QString const& GetKanas() const { return kanas; }
        QString const& GetKanji() const { return kanji; }
        QString const& GetTrad() const { return trad; }
        int GetLearningScore() const { return learningScore; }
        void SetLearningScore(int ls); // Need to save ls into vocabSheetPath
        QString const& GetPath() const { return vocabDataFile->GetPath(); }
//        bool IsSane() { return kanas != "" && kanji != "" && trad != "" && lineNumber != -1; }

    private :
        QString kanas;
        QString kanji;
        QString trad;
        int learningScore;
        VocabDataFile* vocabDataFile; // that way we don't store the same string xxx times for each file entry
        int lineNumber;
};
inline uint qHash(const VocabDataEntry &key, uint seed)
{
    return qHash(key.GetKanas(), seed) ^ qHash(key.GetKanji(), seed+1) ^ qHash(key.GetTrad(), seed+2) ^ qHash(key.GetPath(), seed+3);
}

class VocabDataPool
{
    public :
        VocabDataPool(QString sheetPath);
        VocabDataPool(QStringList sheetPath);
        QSet<VocabDataEntry> const& GetAllEntries() { return entries; }
        QSet<VocabDataEntry>* ModifyAllEntries() { return &entries; }

    private :
        bool DoesLineContain(VocabDataEntry vde);
        QSet<VocabDataEntry> entries;
        QSet<VocabDataEntry> malformedLines;
        QSet<VocabDataFile> files;
};

#endif // VOCABULARYPARSER_H
