#ifndef VOCABEXPLORERPAGE_H
#define VOCABEXPLORERPAGE_H

#include <vector>
#include <QWidget>
#include <QDir>

namespace Ui
{
    class VocabExplorerPage;
}

class VocabFileEntryWidget;
class VocabExplorerPage : public QWidget
{
    Q_OBJECT

public:
    explicit VocabExplorerPage(QWidget *parent = nullptr);
    ~VocabExplorerPage() override;
    void Populate(QDir dir);

    void InitializeVocabularyLearnEditSet();

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private slots:
    void on_SelectAllButton_clicked();
    void on_pushButton_clicked();

private:
    void Populate();

    Ui::VocabExplorerPage *ui;
    std::vector<VocabFileEntryWidget*> vocabFileWidgets;
    void OnSliderReleased() const;
    void OnValueChanged(int /*value*/) const;

    bool selectAllStatus;
    QString currentVocabDirString;
    QDir currentDir;
};

#endif // VOCABEXPLORERPAGE_H
