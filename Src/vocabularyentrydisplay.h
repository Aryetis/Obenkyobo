#ifndef VOCABULARYENTRYDISPLAY_H
#define VOCABULARYENTRYDISPLAY_H

#include <QWidget>
#include "vocabularycfglistentry.h"

namespace Ui
{
class VocabularyEntryDisplay;
}

class VocabularyEntryDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit VocabularyEntryDisplay(VocabularyCfgListEntry vocab, QWidget *parent = nullptr);
    explicit VocabularyEntryDisplay(QWidget *parent = nullptr);
    ~VocabularyEntryDisplay();

private slots:
    void on_backButton_clicked();
    void on_hideButton_clicked();

private:
    Ui::VocabularyEntryDisplay *ui;
};

#endif // VOCABULARYENTRYDISPLAY_H
