#include "QcmPool.h"

#include "Src/VocabularyParser.h"
#include "Src/QcmDataEntryKana.h"

QcmPool::QcmPool(std::vector<QcmDataEntry *> entries_)
    : entries(entries_)
{}

bool QcmPool::WriteLearningScoreTransaction(std::vector<std::pair<int, QcmDataEntry*> > transaction)
{
    bool ret = true;
    std::vector<std::pair<int, QcmDataEntryKana*>> KanaTransactions;
    std::map<VocabDataFile*, std::vector<std::pair<int, VocabDataEntry*>>> VocabTransactions; // One vector of Transaction per VocabDataFile

    for(std::pair<int, QcmDataEntry *>& trans : transaction)
    {
        VocabDataEntry* vde = static_cast<VocabDataEntry*>(trans.second);
        if (vde != nullptr)
            VocabTransactions[vde->GetVocabDataFile()].emplace_back(trans.first, trans.second);
        else
        {
            QcmDataEntryKana* qde = static_cast<QcmDataEntryKana*>(trans.second);
            if (qde != nullptr)
                KanaTransactions.emplace_back(trans.first, trans.second);
            else
                std::cerr << "[WriteLearningScoreTransaction] Unknown QcmDataEntry format for Transaction" << std::endl;
        }
    }

    for(std::pair<int, QcmDataEntryKana*>& trans : KanaTransactions)
        trans.second->LearningScore(trans.first); // don't care it's all in ram

    for(const auto& [key, value] : VocabTransactions)
        ret = ret && key->WriteLearningScore(value);

    return ret;
}
