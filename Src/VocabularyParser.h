#ifndef VOCABULARYPARSER_H
#define VOCABULARYPARSER_H

#include <QString>
#include <QSet>
#include "Src/QcmDataEntry.h"
#include "Src/DefinesLand.h"

class VocabDataEntry;
class VocabDataPool;
class VocabDataFile
{
    public :
        VocabDataFile() = default;
        VocabDataFile(QString path_, VocabDataPool* pool_ = nullptr);
        ~VocabDataFile();
        QString const& GetPath() const { return vocabSheetPath; }
        QSet<VocabDataEntry*>& Entries() { return entries; }
        QSet<VocabDataEntry*>& MalformedLines() { return malformedLines; }
        int GetLearningScore() const { return learningScore; }

        static bool WriteLearningScore(QString vocabSheetPath, int ls, int lineNumber = -1);
        static bool ResetLearningScore(QString vocabSheetPath);

    private :
        void ParseLine(QString const& line, int lineNumber);

        QString vocabSheetPath;
        QSet<VocabDataEntry*> entries;
        QSet<VocabDataEntry*> malformedLines; // nothing is guaranteed to be valid/parsed except LineNumber and VocabDataFile
        VocabDataPool* pool; // "pool" this VocabDataFile is registred in (nothing prevents us registering VDF in multiple pool but not use for now)
        int learningScore;
};

class VocabDataEntry : public QcmDataEntry
{
    public :
        VocabDataEntry() = delete;
        ~VocabDataEntry() {};
        VocabDataEntry(QString kanas_, QString kanjis_, QString trad_, int ls_, VocabDataFile* vocabDataFile_, int lineNumber_, KanaFamilyEnum fontType_) :
            kanas(kanas_), kanjis(kanjis_), trad(trad_), learningState(ls_), vocabDataFile(vocabDataFile_), lineNumber(lineNumber_), fontType(fontType_) {} // TODO : don't copy, move instead

        bool operator==(const VocabDataEntry& rhs) const
        {
            return kanas == rhs.kanas && kanjis == rhs.kanjis && trad == rhs.trad && learningState == rhs.learningState && GetPath() == rhs.GetPath() && lineNumber == rhs.lineNumber;
        }

        QString const* Kanas() const { return &kanas; }
        QString const* Kanjis() const { return &kanjis; }
        QString const* Romanji() const { return &trad; }
        int LearningState() const { return learningState; }
        void LearningState(int ls); // TODO : Need to save ls into vocabSheetPath
        QString const& GetPath() const { return vocabDataFile->GetPath(); }
        int GetLineNumber() const { return lineNumber; }
        KanaFamilyEnum GetFontType() const { return fontType; }

        bool IsEnabled() const;
        void Enabled(bool b);

    private :
        QString kanas;
        QString kanjis;
        QString trad;
        int learningState;
        VocabDataFile* vocabDataFile; // not owned, only a shortcut to not have to store path again
        int lineNumber;
        KanaFamilyEnum fontType;
};
inline uint qHash(const VocabDataEntry &key, uint seed)
{
    return qHash(key.Kanas(), seed) ^ qHash(key.Kanjis(), seed+1) ^ qHash(key.Romanji(), seed+2) ^ qHash(key.GetPath(), seed+3);
}

class VocabDataPool
{
    public :
        VocabDataPool() = default;
        VocabDataPool(QString sheetPath);
        VocabDataPool(QSet<QString> sheetPaths);
        ~VocabDataPool();
        void RemoveVDF(VocabDataFile& vdf); // don't destroy vdf
        QSet<VocabDataEntry*>& AllEntries() { return entries; }
        void PopulateFromPath(QString path);
        void PopulateFromPaths(QSet<QString> sheetPaths);
        void Clear();

    private :
        bool DoesLineContain(VocabDataEntry vde);
        QSet<VocabDataEntry*> entries; // can hold multiple "identical entries" if stored in seperate files (as designed for now)
        QSet<VocabDataEntry*> malformedLines;
        QSet<VocabDataFile*> files;
};

#endif // VOCABULARYPARSER_H
