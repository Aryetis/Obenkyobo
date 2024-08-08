#ifndef EXPLORERPAGE_H
#define EXPLORERPAGE_H

#include <vector>
#include <QWidget>
#include <QDir>
#include <QTimer>
#include <QList>

namespace Ui
{
    class ExplorerPage;
}

class BaseVocabFileEntryWidget;

class BaseExplorerPage : public QWidget
{
    Q_OBJECT

public:
    void Populate(QDir dir);
    void OnSleep() const;
    void OnWakeUp() const;

    void InitializePage();
    bool IsScrollBarDisplayed() const;

protected:
    BaseExplorerPage(QString curDirSerializedAdress_, QList<QString> extensions_, QWidget *parent = nullptr);
    ~BaseExplorerPage() override;

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
    void DeleteVocabWidgets();

    Ui::ExplorerPage *ui;
    std::vector<BaseVocabFileEntryWidget*> vocabWidgets;
    void OnSliderReleased() const;
    void OnValueChanged(int /*value*/) const;

    bool initialPaintDone;
    bool selectAllStatus;
    QDir currentDir;

    QTimer homeLongPressTimer;

    // Children specific
    QString curDirSerializedAdress;
    QList<QString> extensions;
    QString tutorial;
};

class VocabExplorerPage : public BaseExplorerPage
{
    Q_OBJECT

public:
    VocabExplorerPage(QWidget *parent = nullptr);
    ~VocabExplorerPage() override = default;
};

class NoteExplorerPage : public BaseExplorerPage
{
    Q_OBJECT

public:
    NoteExplorerPage(QWidget *parent = nullptr);
    ~NoteExplorerPage() override = default;
};

#endif // EXPLORERPAGE_H
