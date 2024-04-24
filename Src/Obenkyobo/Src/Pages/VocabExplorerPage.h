#ifndef VOCABEXPLORERPAGE_H
#define VOCABEXPLORERPAGE_H

#include <vector>
#include <QWidget>
#include <QDir>
#include <QTimer>

namespace Ui
{
    class VocabExplorerPage;
}

class VocabBaseEntryWidget;
class VocabExplorerPage : public QWidget
{
    Q_OBJECT

public:
    explicit VocabExplorerPage(QWidget *parent = nullptr);
    ~VocabExplorerPage() override;

    void Populate(QDir dir);
    void OnSleep() const;
    void OnWakeUp() const;

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
    void HomeButtonLongPressReleased();
    void HomeButtonLongPressAction();

    Ui::VocabExplorerPage *ui;
    std::vector<VocabBaseEntryWidget*> vocabWidgets;
    void OnSliderReleased() const;
    void OnValueChanged(int /*value*/) const;

    bool initialPaintDone;
    bool selectAllStatus;
    QDir currentDir;

    QTimer homeLongPressTimer;
};

#endif // VOCABEXPLORERPAGE_H
