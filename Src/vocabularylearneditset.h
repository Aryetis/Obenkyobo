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
    ~VocabularyLearnEditSet() override;

    void InitializeVocabularyLearnEditSet();

private slots:
    void on_SelectAllButton_clicked();

private:
    Ui::VocabularyLearnEditSet *ui;
    std::vector<VocabularyCfgListEntry*> vocabCfgs;
    void OnSliderReleased() const;
    void OnValueChanged(int /*value*/) const;

    bool selectAllStatus;
};

#endif // VOCABULARYLEARNEDITSET_H
