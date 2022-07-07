#ifndef VOCABULARYDISPLAYPAGE_H
#define VOCABULARYDISPLAYPAGE_H

#include <QWidget>
#include <QList>
#include <QPushButton>
#include "Src/Widgets/VocabFileEntryWidget.h"
#include "Src/KanasTables.h"

namespace Ui
{
    class VocabularyDisplayPage;
}

struct tempVocab // TODO add to some "vocab entry set" => refactor qcm
{
    KanaFamilyEnum fontType;
    QString kanas;
    QString kanji;
    QString trad;
    int learningScore;
    QPushButton* labels[4] = {nullptr};

    tempVocab() = delete;
    tempVocab(KanaFamilyEnum sfe, QString kanas_, QString kanji_, QString t, int ls)
        : fontType(sfe), kanas(kanas_), kanji(kanji_), trad(t), learningScore(ls)
    { }
};

class VocabularyDisplayPage : public QWidget
{
    Q_OBJECT

public:
    explicit VocabularyDisplayPage(QWidget *parent = nullptr);
    ~VocabularyDisplayPage();

    void InitializeGrid(VocabFileEntryWidget* vocab);
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

    Ui::VocabularyDisplayPage *ui;
    QList<tempVocab*> gridEntries;

    int curPage;
    int maxPage;
    bool kanasShow;
    bool kanjiShow;
    bool traductionShow;
    bool lsShow;
};

#endif // VOCABULARYDISPLAYPAGE_H
