#ifndef QCMDATAENTRY_H
#define QCMDATAENTRY_H

#include <QString>

class KanasTableFamily;
class QcmDataEntry
{
public :
    virtual bool RegisterAndInitializeSerializedVals(QString serializedAddress, KanasTableFamily* _p) = 0;

//    virtual bool operator==(QcmDataEntry const & rhs) const = 0;

    virtual QString const* Kanas() const = 0;
    virtual QString const* Kanjis() const = 0;
    virtual QString const* Romanji() const = 0;

    virtual bool IsEnabled() const = 0;
    virtual void Enabled(bool b) = 0;

    virtual int LearningState() const = 0;
    virtual void LearningState(int ls) = 0;
};

#endif // QCMDATAENTRY_H
