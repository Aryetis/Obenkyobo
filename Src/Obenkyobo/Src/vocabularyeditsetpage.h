#ifndef VOCABULARYEDITSETPAGE_H
#define VOCABULARYEDITSETPAGE_H

#include <QWidget>
#include <QDialog>
#include <QSettings>
#include <vector>

namespace Ui
{
    class VocabularyEditSetPage;
}

class VocabularyEditSetPage : public QDialog
{
    Q_OBJECT

public:
    explicit VocabulaQSettingsryEditSetPage(QWidget *parent = nullptr);
    ~VocabularyEditSetPage() override;

private:
    Ui::VocabularyEditSetPage *ui;
    std::vector<QSettings> vocabCfgs;
};

#endif // VOCABULARYEDITSETPAGE_H
