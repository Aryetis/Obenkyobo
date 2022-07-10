#ifndef VOCABULARYDISPLAYPAGE_H
#define VOCABULARYDISPLAYPAGE_H

#include <QWidget>
#include <QList>
#include <QPushButton>
#include "Src/Widgets/VocabFileEntryWidget.h"
#include "Src/KanasTables.h"
#include "Src/VocabularyParser.h"

namespace Ui
{
    class VocabularyDisplayPage;
}

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

    VocabDataFile* vdf;
    Ui::VocabularyDisplayPage *ui;
    std::vector<std::vector<QPushButton*>> gridLabels; // std::vector<std::vector<{kana, kanji, trad, LS}>>

    int curPage;
    int maxPage;
    bool kanasShow;
    bool kanjiShow;
    bool traductionShow;
    bool lsShow;
};

#endif // VOCABULARYDISPLAYPAGE_H
