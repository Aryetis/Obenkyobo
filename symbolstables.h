#ifndef SYMBOLSTABLES_H
#define SYMBOLSTABLES_H

#include <vector>
#include <string>
#include <QStringLiteral>
#include <QString>
#include <QPalette>
#include "GetMy.h"

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

        enabledSerializedAddress = "Symbols/enabled_"+serializedAddress+"_"+romanji;
        enabled = GetMy::GetInstance().SettingSerializer()->value(enabledSerializedAddress, true).toBool();
        learningStateSerializedAddress = "Symbols/learningState"+serializedAddress+"_"+romanji;
        learningState = GetMy::GetInstance().SettingSerializer()->value(learningStateSerializedAddress, 0).toInt();
        return enabled;
    }

    bool operator==(Symbol const & rhs) const
    {
        return (this->romanji== rhs.romanji && this->jp == rhs.jp);
    }

    const QString& JP() const { return jp; }
    const QString& Romanji() const {return romanji;}

    bool Enabled() const { return enabled; }
    void Enabled(bool b); // inlined at the end because of parentedFamily->NbrOfEnabled

    int LearningState() const { return learningState; }
    void LearningState(int ls)
    {
        learningState = ls;
        GetMy::GetInstance().SettingSerializer()->setValue(learningStateSerializedAddress, learningState);
    }

private :
    QString romanji;
    QString jp;
    QString enabledSerializedAddress;
    bool enabled;
    QString learningStateSerializedAddress;
    int learningState; // 0 <=> unknown, [1;3] learning, [4;5] learned)
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
                if (s.RegisterAndInitializeSerializedVals(name+"_"+sts.Name(), this))
                    ++nbrOfEnabled;
    }

    std::vector<SymbolsTableSection>& Data() { return data; } // not const as we need to adjust Symbols's LearningState
    void NbrOfEnabled(int _n) { nbrOfEnabled = _n; }
    int NbrOfEnabled() const { return nbrOfEnabled; }

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
        GetMy::GetInstance().SettingSerializer()->setValue(enabledSerializedAddress, enabled);
        (b) ? parentedFamily->NbrOfEnabled(parentedFamily->NbrOfEnabled()+1)
            : parentedFamily->NbrOfEnabled(parentedFamily->NbrOfEnabled()-1);
    }
}



//================================== SymbolsTables ==================================
namespace SymbolsTables
{
    extern SymbolsTableFamily HiraganaSymbolsTableFamily;
    extern SymbolsTableFamily KatakanaSymbolsTableFamily;
}

#endif // SYMBOLSTABLES_H
