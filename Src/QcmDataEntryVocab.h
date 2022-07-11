#ifndef QCMDATAENTRYVOCAB_H
#define QCMDATAENTRYVOCAB_H

#include "Src/QcmDataEntry.h"

class QcmDataEntryVocab : public QcmDataEntry
{
public :
    QcmDataEntryVocab() = delete;
    QcmDataEntryVocab(QString romanji_, QString kanas_, QString kanjis_)
        : romanji(romanji_), kanas(kanas_), kanjis(kanjis_) {}

    bool RegisterAndInitializeSerializedVals(QString serializedAddress, KanasTableFamily* _p);

    bool operator==(QcmDataEntryVocab const & rhs) const;

    QString const* Kanas() const { return &kanas; }
    QString const* Kanjis() const { return &kanjis; }
    QString const* Romanji() const { return &romanji; }

    bool IsEnabled() const { return enabled; }
    void Enabled(bool b);

    int LearningState() const { return learningState; }
    void LearningState(int ls);

private :
    QString romanji;
    QString kanas;
    QString kanjis;
    bool enabled;
    int learningState; // [0;1] learned, [2;4] learning, 5 unknown
};
#endif // QCMDATAENTRYVOCAB_H
