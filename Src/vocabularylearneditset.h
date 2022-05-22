#ifndef VOCABULARYLEARNEDITSET_H
#define VOCABULARYLEARNEDITSET_H

#include <vector>
#include <QWidget>
#include <QDir>
#include <QSpacerItem>
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
    void Populate(QDir dir);

    void InitializeVocabularyLearnEditSet();

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private slots:
    void on_SelectAllButton_clicked();

    void on_pushButton_clicked();

private:
    void Populate();

    Ui::VocabularyLearnEditSet *ui;
    std::vector<VocabularyCfgListEntry*> vocabCfgs;
    void OnSliderReleased() const;
    void OnValueChanged(int /*value*/) const;

    bool selectAllStatus;
    QString currentVocabDirString;
    QDir currentDir;
};

#endif // VOCABULARYLEARNEDITSET_H
