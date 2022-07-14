#ifndef QCMDATAENTRYKANA_H
#define QCMDATAENTRYKANA_H

#include "Src/QcmDataEntry.h"

class QcmDataEntryKana : public QcmDataEntry // previously named Symbol
{
public :
    QcmDataEntryKana() = delete;
    ~QcmDataEntryKana() = default;
    QcmDataEntryKana(QString romanji_, QString kanas_) : romanji(romanji_), kanas(kanas_) {}

    bool RegisterAndInitializeSerializedVals(QString serializedAddress, KanasTableFamily* _p);

    bool operator==(QcmDataEntryKana const & rhs) const;

    QString const* Kanas() const { return &kanas; }
    QString const* Kanjis() const { return nullptr; }
    QString const* Romanji() const { return &romanji; }

    bool IsEnabled() const { return enabled; }
    void Enabled(bool b);

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

#endif // QCMDATAENTRYKANA_H
