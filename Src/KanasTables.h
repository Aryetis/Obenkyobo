#ifndef KANASTABLES_H
#define KANASTABLES_H

#include <vector>
#include <string>
#include <QString>
#include "Src/GetMy.h"
#include "Src/QcmDataEntry.h"
#include "Src/QcmDataEntryKana.h"

//================================== SymbolsTableSection ==================================
class SymbolsTableSection
{
public :
    SymbolsTableSection(QString _name, std::vector<QcmDataEntryKana> _data, int _el)
        : sectionName(_name), data(_data), elementPerColumnInDisplaySet(_el)
    {}

    const QString& Name() const { return sectionName; }
    std::vector<QcmDataEntryKana>& Data() { return data; } // not const as we need to adjust their LearningState
    int ElementPerColumnInDisplaySet() { return elementPerColumnInDisplaySet; }

private :
    QString sectionName;
    std::vector<QcmDataEntryKana> data;
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
