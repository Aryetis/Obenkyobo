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
    bool IsScrollBarDisplayed() const;

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_SelectAllButton_clicked();
    void on_homeButton_clicked();

private:
    void Populate();
    void SetAndTrimCurDirLabel();

    Ui::VocabExplorerPage *ui;
    std::vector<VocabFileEntryWidget*> vocabFileWidgets;
    void OnSliderReleased() const;
    void OnValueChanged(int /*value*/) const;

    bool initialPaintDone;
    bool selectAllStatus;
    QString currentVocabDirString;
    QDir currentDir;
};

#endif // VOCABEXPLORERPAGE_H
