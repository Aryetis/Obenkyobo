#ifndef VOCABULARYDISPLAY_H
#define VOCABULARYDISPLAY_H

#include <QWidget>
#include <QList>
#include <QLabel>
#include "vocabularycfglistentry.h"
#include "symbolstables.h"

namespace Ui
{
    class VocabularyDisplay;
}

struct tempVocab // TODO add to some "vocab entry set" => refactor qcm
{
    SymbolFamilyEnum fontType;
    QString kanas;
    QString kanji;
    QString trad;
    int learningScore;
    QLabel* labels[4] = {nullptr};

    tempVocab() = delete;
    tempVocab(SymbolFamilyEnum sfe, QString kanas_, QString kanji_, QString t, int ls)
        : fontType(sfe), kanas(kanas_), kanji(kanji_), trad(t), learningScore(ls)
    { }
};

class VocabularyDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit VocabularyDisplay(QWidget *parent = nullptr);
    ~VocabularyDisplay();

    void InitializeGrid(VocabularyCfgListEntry* vocab);

private slots:
    void on_randomizeButton_clicked();

private:
    void PopulateGrid(bool random = false);

    Ui::VocabularyDisplay *ui;
    QList<tempVocab*> gridEntries;
    QFont curHiraganaNonSized;
    QFont curKatakanaNonSized;
};

#endif // VOCABULARYDISPLAY_H
