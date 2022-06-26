#ifndef SYMBOLSTABLES_H
#define SYMBOLSTABLES_H

#include <vector>
#include <string>
#include <QStringLiteral>
#include <QString>
#include <QPalette>
#include "Src/GetMy.h"

#define MAX_LEARNING_STATE_VALUE 5

enum SymbolFamilyEnum
{
    hiragana,
    katakana,
    kanji // meh we'll see during refacto
};

//================================== Symbol ==================================
class SymbolsTableFamily;
class Symbol
{
public :
    Symbol() = default;
    Symbol(QString r_, QString j_)
        :romanji(r_), jp(j_)
    {}

    bool RegisterAndInitializeSerializedVals(QString serializedAddress, SymbolsTableFamily* _p)
    {
        parentedFamily = _p;

        enabledSerializedAddress = ("Symbols/enabled_"+serializedAddress+"_"+romanji).toUtf8();
        enabled = GetMy::Instance().SettingSerializer()->value(enabledSerializedAddress, true).toBool();
        learningStateSerializedAddress = ("Symbols/learningState"+serializedAddress+"_"+romanji).toUtf8();
        learningState = GetMy::Instance().SettingSerializer()->value(learningStateSerializedAddress, 5).toInt();
        return enabled;
    }

    bool operator==(Symbol const & rhs) const
    {
        return (this->romanji== rhs.romanji && this->jp == rhs.jp);
    }

    const QString& JP() const { return jp; }
    const QString& Romanji() const {return romanji;}

    bool IsEnabled() const { return enabled; }
    void Enabled(bool b); // inlined at the end because of parentedFamily->NbrOfEnabled

    int LearningState() const { return learningState; }
    void LearningState(int ls);

    static int GetMaxlearningState() { return MAX_LEARNING_STATE_VALUE; }

private :
    QString romanji;
    QString jp;
    QString enabledSerializedAddress;
    bool enabled;
    QString learningStateSerializedAddress;
    int learningState; // [0;1] learned, [2;4] learning, 5 unknown)
    SymbolsTableFamily* parentedFamily;
};


//================================== SymbolsTableSection ==================================
class SymbolsTableSection
{
public :
    SymbolsTableSection(QString _name, std::vector<Symbol> _data, int _el)
        : sectionName(_name), data(_data), elementPerColumnInDisplaySet(_el)
    {}

    const QString& Name() const { return sectionName; }
    std::vector<Symbol>& Data() { return data; } // not const as we need to adjust their LearningState
    int ElementPerColumnInDisplaySet() { return elementPerColumnInDisplaySet; }

private :
    QString sectionName;
    std::vector<Symbol> data;
    int elementPerColumnInDisplaySet;
};



//================================== SymbolsTableFamily ==================================
class SymbolsTableFamily
{
public :
    SymbolsTableFamily(QString _name, std::vector<SymbolsTableSection> _data)
        : name(_name), data(_data), nbrOfEnabled(0)
    {}

    void InitializeSerializedVals() // Can't do it in constructor because SettingSerializer isn't set yet.
    {
        for (SymbolsTableSection& sts : data)
            for (Symbol& s : sts.Data())
                if (s.RegisterAndInitializeSerializedVals(name, this))
                    ++nbrOfEnabled;
    }

    std::vector<SymbolsTableSection>& Data() { return data; } // not const as we need to adjust Symbols's LearningState
    void NbrOfEnabled(int _n) { nbrOfEnabled = _n; }
    int NbrOfEnabled() const { return nbrOfEnabled; }
    void ResetWeights()
    {
        for (SymbolsTableSection& sts : data)
            for (Symbol& s : sts.Data())
                s.LearningState(MAX_LEARNING_STATE_VALUE);
    }

private :
    QString name;
    std::vector<SymbolsTableSection> data;
    int nbrOfEnabled;
};

inline void Symbol::Enabled(bool b)
{
    if (b != enabled)
    {
        enabled = b;
        GetMy::Instance().SettingSerializer()->setValue(enabledSerializedAddress, enabled);
        if (enabled)
            parentedFamily->NbrOfEnabled(parentedFamily->NbrOfEnabled()+1);
        else
            parentedFamily->NbrOfEnabled(parentedFamily->NbrOfEnabled()-1);
    }
}

inline void Symbol::LearningState(int ls)
{
    learningState = ls;
    GetMy::Instance().SettingSerializer()->setValue(learningStateSerializedAddress, learningState);
}



//================================== SymbolsTables ==================================
namespace SymbolsTables
{
    extern SymbolsTableFamily HiraganaSymbolsTableFamily;
    extern SymbolsTableFamily KatakanaSymbolsTableFamily;
    extern SymbolsTableFamily VocabTableFamily; // TODO
}

#endif // SYMBOLSTABLES_H
