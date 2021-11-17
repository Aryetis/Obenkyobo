#ifndef VOCABULARYLEARNEDITSET_H
#define VOCABULARYLEARNEDITSET_H

#include <vector>
#include <QWidget>
#include "vocabularycfglistentry.h"

namespace Ui
{
class VocabularyLearnEditSet;
}

class VocabularyLearnEditSet : public QWidget
{
    Q_OBJECT

public:
    explicit VocabularyLearnEditSet(QWidget *parent = nullptr);
    ~VocabularyLearnEditSet();

private:
    Ui::VocabularyLearnEditSet *ui;
    std::vector<VocabularyCfgListEntry*> vocabCfgs;
};

#endif // VOCABULARYLEARNEDITSET_H
