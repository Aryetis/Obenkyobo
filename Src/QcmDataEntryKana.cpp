#include "QcmDataEntryKana.h"
#include "Src/GetMy.h"
#include "Src/KanasTables.h"

bool QcmDataEntryKana::RegisterAndInitializeSerializedVals(QString serializedAddress, KanasTableFamily *_p)
{
    parentedFamily = _p;

    enabledSerializedAddress = ("Symbols/enabled_"+serializedAddress+"_"+romanji).toUtf8();
    enabled = GetMy::Instance().SettingSerializerInst()->value(enabledSerializedAddress, true).toBool();
    learningStateSerializedAddress = ("Symbols/learningState"+serializedAddress+"_"+romanji).toUtf8();
    learningState = GetMy::Instance().SettingSerializerInst()->value(learningStateSerializedAddress, 5).toInt();
    return enabled;
}

bool QcmDataEntryKana::operator==(const QcmDataEntryKana &rhs) const
{
    return (this->romanji== rhs.romanji && this->kanas == rhs.kanas);
}

void QcmDataEntryKana::Enabled(bool b)
{
    if (b != enabled)
    {
        enabled = b;
        GetMy::Instance().SettingSerializerInst()->setValue(enabledSerializedAddress, enabled);
        if (enabled)
            parentedFamily->NbrOfEnabled(parentedFamily->NbrOfEnabled()+1);
        else
            parentedFamily->NbrOfEnabled(parentedFamily->NbrOfEnabled()-1);
    }
}

void QcmDataEntryKana::LearningState(int ls)
{
    learningState = ls;
    GetMy::Instance().SettingSerializerInst()->setValue(learningStateSerializedAddress, learningState);
}
