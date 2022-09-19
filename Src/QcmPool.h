#ifndef QCMPOOL_H
#define QCMPOOL_H

#include <vector>
#include <Src/QcmDataEntry.h>

// Can mix and match both VocabDataEntry and QcmDataEntryKana (not used at the moment)
class QcmPool
{
public:
    QcmPool() = delete;
    QcmPool(std::vector<QcmDataEntry*> entries_);
    ~QcmPool() = default;

    bool WriteLearningScoreTransaction(std::vector<std::pair<int, QcmDataEntry*> > transaction);

private:
    std::vector<QcmDataEntry*> entries;
};

#endif // QCMPOOL_H
