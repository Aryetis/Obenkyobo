#ifndef VOCABULARYDISPLAY_H
#define VOCABULARYDISPLAY_H

#include <QWidget>
#include <QList>
#include <QPushButton>
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
    QPushButton* labels[4] = {nullptr};

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
    void CleanGrid();

private slots:
    void on_randomizeButton_clicked();
    void on_nextPageButton_clicked();
    void on_previousPageButton_clicked();
    void on_KanasHidePushButton_clicked();
    void on_KanjiHidePushButton_clicked();
    void on_TraductionHidePushButton_clicked();
    void on_LSHidePushButton_clicked();

private:
    void PopulateGrid(bool random = false, int turnPage = 0);
    void HideColumn(int col, bool b);

    Ui::VocabularyDisplay *ui;
    QList<tempVocab*> gridEntries;

    int curPage;
    int maxPage;
    bool kanasShow;
    bool kanjiShow;
    bool traductionShow;
    bool lsShow;

};

#endif // VOCABULARYDISPLAY_H
