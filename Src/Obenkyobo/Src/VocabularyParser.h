#ifndef VOCABULARYPARSER_H
#define VOCABULARYPARSER_H

#include <QString>
#include <QSet>
#include <QFont>
#include "Src/QcmDataEntry.h"
#include "Src/DefinesLand.h"



                                    #include <iostream>

// Everything marked as Lnk is a shortcut => isn't owned by this => isn't to be destroyed by this
// SmartPointers ? What's that ?

class VocabDataPool;
class VocabDataEntry;
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
        QSet<VocabDataPool*> const& GetPoolLnks() { return poolLnks; }
        bool ResetLearningScore();
        bool WriteLearningScore(std::vector<std::pair<int, VocabDataEntry*>> transaction);

    private :
        bool WriteLearningScoreInt(std::vector<std::pair<int, int>> transaction);
        void ParseLine(QString const& line, int lineNumber);

        QString vocabSheetPath;
        QSet<VocabDataEntry*> entries;
        QSet<VocabDataEntry*> malformedLines; // nothing is guaranteed to be valid/parsed except LineNumber and VocabDataFile
        QSet<VocabDataPool*> poolLnks; // "pool" "this" is registred in (can't really register to multiple pools for now, but no need for it too)
        int learningScore;
};

class VocabDataEntry : public QcmDataEntry
{
    // Necessary so Write/ResetLearningScore can update VDE fields appropiately and keep VDF::learningScore updated
    // Regardless if modifications are coming from a VDE or a VDF
    friend class VocabDataFile;

    public :
        VocabDataEntry() = delete;
        ~VocabDataEntry() = default;
        VocabDataEntry(QString kanas_, QString kanjis_, QString trad_, int ls_, VocabDataFile* vocabDataFile_, int lineNumber_, KanaFamilyEnum fontType_) :
            kanas(kanas_), kanjis(kanjis_), trad(trad_), learningScore(ls_), vocabDataFileLnk(vocabDataFile_), lineNumber(lineNumber_), fontType(fontType_)
        {} // TODO : don't copy, move instead... not quite sure if it's a good plan... will do for now

        bool operator==(const VocabDataEntry& rhs) const
        {
std::cout << "operator equals VocabDataEntry& member" << std::endl;
            return kanas == rhs.kanas && kanjis == rhs.kanjis && trad == rhs.trad && learningScore == rhs.learningScore && GetPath() == rhs.GetPath() && lineNumber == rhs.lineNumber;
        }

        QString const* Kanas() const { return &kanas; }
        QString const* Kanjis() const { return &kanjis; }
        QString const* Romanji() const { return &trad; }
        int LearningScore() const { return learningScore; }
        void LearningScore(int ls);
        QString const& GetPath() const { return vocabDataFileLnk->GetPath(); }
        VocabDataFile* GetVocabDataFile() const { return vocabDataFileLnk; }
        int GetLineNumber() const { return lineNumber; }
        KanaFamilyEnum GetFontType() const { return fontType; }

    private :
        QString kanas;
        QString kanjis;
        QString trad;
        int learningScore;
        VocabDataFile* vocabDataFileLnk;
        int lineNumber;
        KanaFamilyEnum fontType;
};
inline uint qHash(const VocabDataEntry* key, uint seed)
{
std::cout << "!!!!!!!!!!!!!!!!!! qHash VocabDataEntry*" << std::endl;  // TODO NOW : why is it never called ?
return 0;
    // return qHash(key.Kanas(), seed) ^ qHash(key.Kanjis(), seed+1) ^ qHash(key.Romanji(), seed+2) ^ qHash(key.GetPath(), seed+3);
    return qHash(key->Kanas(), seed) ^ qHash(key->Kanjis(), seed+1);
}
inline bool operator==(const VocabDataEntry& lhs, const VocabDataEntry& rhs)
{
std::cout << "!!!!!!!!!!!!!!!!!! operator equals VocabDataEntry& inline" << std::endl; // TODO NOW : why is it never called ?
    return lhs.Kanas() == rhs.Kanas() && lhs.Kanjis() == rhs.Kanjis();
}

class VocabDataPool
{
    public :
        VocabDataPool() = default;
        VocabDataPool(QString sheetPath);
        VocabDataPool(QSet<QString> sheetPaths);
        ~VocabDataPool();
        void RemoveVDF(VocabDataFile* vdf); // don't destroy vdf
        QSet<VocabDataEntry*>& AllEntries() { return entriesLnks; }
        void PopulateFromPath(QString path);
        void PopulateFromPaths(QSet<QString> sheetPaths);
        void Clear();

        void DebugLog();

    private :
        bool DoesLineContain(VocabDataEntry vde);
        QSet<VocabDataEntry*> entriesLnks; // SHORCUTS ; can hold multiple "identical entries" if stored in seperate files (as designed for now)
        QSet<VocabDataEntry*> malformedLinesLnks; // TODO : not sure if it's really useful to store... might get away with storing only lineNumber
        QSet<VocabDataFile*> files;
};

#endif // VOCABULARYPARSER_H
