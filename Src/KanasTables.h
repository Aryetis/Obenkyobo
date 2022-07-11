#ifndef KANASTABLES_H
#define KANASTABLES_H

#include <vector>
#include <string>
#include <QString>
#include "Src/GetMy.h"
#include "Src/QcmDataEntry.h"

//================================== Symbol ==================================
// TODO NOW : adapt and rename to work with VocabularyData
class KanasTableFamily;
class Kana : public QcmDataEntry
{
public :
    Kana() = delete;
    Kana(QString romanji_, QString kanas_) : romanji(romanji_), kanas(kanas_) {}

    bool RegisterAndInitializeSerializedVals(QString serializedAddress, KanasTableFamily* _p);

    bool operator==(Kana const & rhs) const;

    QString const* Kanas() const { return &kanas; }
    QString const* Kanjis() const { return nullptr; }
    QString const* Romanji() const { return &romanji; }

    bool IsEnabled() const { return enabled; }
    void Enabled(bool b); // inlined at the end because of parentedFamily->NbrOfEnabled

    int LearningState() const { return learningState; }
    void LearningState(int ls);

private :
    QString romanji;
    QString kanas;
    QString enabledSerializedAddress;
    bool enabled;
    QString learningStateSerializedAddress;
    int learningState; // [0;1] learned, [2;4] learning, 5 unknown
    KanasTableFamily* parentedFamily;
};


//================================== SymbolsTableSection ==================================
class SymbolsTableSection
{
public :
    SymbolsTableSection(QString _name, std::vector<Kana> _data, int _el)
        : sectionName(_name), data(_data), elementPerColumnInDisplaySet(_el)
    {}

    const QString& Name() const { return sectionName; }
    std::vector<Kana>& Data() { return data; } // not const as we need to adjust their LearningState
    int ElementPerColumnInDisplaySet() { return elementPerColumnInDisplaySet; }

private :
    QString sectionName;
    std::vector<Kana> data;
    int elementPerColumnInDisplaySet;
};

//================================== SymbolsTableFamily ==================================
class KanasTableFamily
{
public :
    KanasTableFamily(QString _name, std::vector<SymbolsTableSection> _data)
        : name(_name), data(_data), nbrOfEnabled(0)
    {}

    void InitializeSerializedVals();

    std::vector<SymbolsTableSection>& Data() { return data; } // not const as we need to adjust Symbols's LearningState
    void NbrOfEnabled(int _n) { nbrOfEnabled = _n; }
    int NbrOfEnabled() const { return nbrOfEnabled; }
    void ResetWeights();

private :
    QString name;
    std::vector<SymbolsTableSection> data;
    int nbrOfEnabled;
};


//================================== KanasTables ==================================
namespace KanasTables
{
    extern KanasTableFamily HiraganaSymbolsTableFamily;
    extern KanasTableFamily KatakanaSymbolsTableFamily;
}

#endif // KANASTABLES_H
