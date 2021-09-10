//#ifndef SYMBOLS_H
//#define SYMBOLS_H

//#include <QString>
//#include "GetMy.h"
//#include "symbolstables.h"

//class SymbolsTableFamily;
//class  Symbol
//{
//public :
//    Symbol() = default;
//    Symbol(QString r_, QString j_)
//        :romanji(r_), jp(j_)
//    {}

//    bool InitializeSerializedVals(QString serializedAddress)
//    {
//        enabledSerializedAddress = "Symbols/enabled_"+serializedAddress+"_"+romanji;
//        enabled = GetMy::GetInstance().SettingSerializer()->value(enabledSerializedAddress, true).toBool();
//        learningStateSerializedAddress = "Symbols/learningState"+serializedAddress+"_"+romanji;
//        learningState = GetMy::GetInstance().SettingSerializer()->value(learningStateSerializedAddress, 0).toInt();
//        return enabled;
//    }

//    bool operator==(Symbol const & rhs) const
//    {
//        return (this->romanji== rhs.romanji && this->jp == rhs.jp);
//    }

//    const QString& JP() const { return jp; }
//    const QString& Romanji() const {return romanji;}

//    bool Enabled() const { return enabled; }
//    void Enabled(bool b)
//    {
//        if (b != enabled)
//        {
//            enabled = b;
//            GetMy::GetInstance().SettingSerializer()->setValue(enabledSerializedAddress, enabled);
//            (b) ? parentedFamily->NbrOfEnabled((parentedFamily->NbrOfEnabled())+1)
//                : parentedFamily->NbrOfEnabled((parentedFamily->NbrOfEnabled())-1);
//        }
//    }

//    int LearningState() const { return learningState; }
//    void LearningState(int ls)
//    {
//        learningState = ls;
//        GetMy::GetInstance().SettingSerializer()->setValue(learningStateSerializedAddress, learningState);
//    }

//private :
//    QString romanji;
//    QString jp;
//    QString enabledSerializedAddress;
//    bool enabled;
//    QString learningStateSerializedAddress;
//    int learningState; // TODO turn this into a std::vector<int> and use with std::discrete_distribution to get actually weighted random result
//                          // NO, that's not the correct approach as enabling/disabling Symbols would shift the indexes giving incorrect results
//                          // => store learningState as int [0;5] with 0<=>Unknown, [1;4]<=>learning, 5<=> learned
//    SymbolsTables::SymbolsTableFamily* parentedFamily;
//};

//#endif // SYMBOLS_H
