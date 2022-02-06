#ifndef VOCABULARYDISPLAY_H
#define VOCABULARYDISPLAY_H

#include <QWidget>
#include <QList>
#include <optional>
#include "vocabularycfglistentry.h"
#include "symbolstables.h"

namespace Ui
{
    class VocabularyDisplay;
}

struct tempVocab // TODO add to some "vocab entry set" => refactor qcm
{
    SymbolFamilyEnum fontType;
    QString jp;
    QString kana;
    QString trad;
    int learningScore;

    tempVocab() = delete;
    tempVocab(SymbolFamilyEnum sfe, QString j, QString k, QString t, int ls)
        : fontType(sfe), jp(j), kana(k), trad(t), learningScore(ls)
    {}
};

class VocabularyDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit VocabularyDisplay(QWidget *parent = nullptr);
    ~VocabularyDisplay();

    void InitializeGrid(VocabularyCfgListEntry* vocab);

private slots:

private:
    Ui::VocabularyDisplay *ui;
    QList<tempVocab*> gridEntries;
};

#endif // VOCABULARYDISPLAY_H
