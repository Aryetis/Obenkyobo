#ifndef SYMBOLSTABLES_H
#define SYMBOLSTABLES_H

#include <vector>
#include <string>
#include <QStringLiteral>
#include <QString>
#include <QPalette>
#include "GetMy.h"

// TODO handle disable / enable set => handle edge cases like "not enough data to initialize entriesGridLayout", etc
class  Symbol
{
public :
    Symbol() = default;
    Symbol(QString r_, QString j_)
        :romanji(r_), jp(j_)
    {}

    void InitializeSerializedVals(QString serializedAddress)
    {
        enabledSerializedAddress = "Symbols/enabled_"+serializedAddress+"_"+romanji;
        enabled = GetMy::GetInstance().SettingSerializer()->value(enabledSerializedAddress, true).toBool();
        learningStateSerializedAddress = "Symbols/learningState"+serializedAddress+"_"+romanji;
        learningState = GetMy::GetInstance().SettingSerializer()->value(learningStateSerializedAddress, 0).toInt();
    }

    bool operator==(Symbol const & rhs) const
    {
        return (this->romanji== rhs.romanji && this->jp == rhs.jp);
    }

    const QString& JP() const { return jp; }
    const QString& Romanji() const {return romanji;}

    bool Enabled() const { return enabled; }
    void Enabled(bool b)
    {
        enabled = b;
        GetMy::GetInstance().SettingSerializer()->setValue(enabledSerializedAddress, enabled);
    }

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
    int learningState; // TODO turn this into a std::vector<int> and use with std::discrete_distribution to get actually weighted random result
                          // NO, that's not the correct approach as enabling/disabling Symbols would shift the indexes giving incorrect results
                          // => store learningState as int [0;5] with 0<=>Unknown, [1;4]<=>learning, 5<=> learned
};



namespace SymbolsTables
{
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



    class SymbolsTableFamily
    {
public :
        SymbolsTableFamily(QString _name, std::vector<SymbolsTableSection> _data)
            : name(_name), data(_data)
        {}

        void InitializeSerializedVals() // Can't do it in constructor because SettingSerializer isn't set yet.
        {
            for (SymbolsTableSection& sts : data)
                for (Symbol& s : sts.Data())
                    s.InitializeSerializedVals(name+"_"+sts.Name());
        }

        std::vector<SymbolsTableSection>& Data() { return data; } // not const as we need to adjust Symbols's LearningState

private :
        QString name;
        std::vector<SymbolsTableSection> data;
    };

    extern SymbolsTableFamily HiraganaSymbolsTableFamily;
    extern SymbolsTableFamily KatakanaSymbolsTableFamily;

}

#endif // SYMBOLSTABLES_H
