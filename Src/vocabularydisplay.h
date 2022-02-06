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
    std::optional<SymbolFamilyEnum> fontType;
    std::optional<QString> jp;
    std::optional<QString> trad;
    std::optional<int> learningScore;

    bool IsSane()
    { return fontType.has_value() && jp.has_value() && trad.has_value() && learningScore.has_value(); }
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
