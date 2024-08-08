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

class BaseFileEntryWidget;

class BaseExplorerPage : public QWidget
{
    Q_OBJECT

public:
    void Populate(QDir dir);
    void OnSleep() const;
    void OnWakeUp() const;

    void InitializePage();
    bool IsScrollBarDisplayed() const;
    void OnSliderReleased() const;
    void OnValueChanged(int /*value*/) const;

protected:
    BaseExplorerPage(QString curDirSerializedAdress_, QWidget *parent = nullptr);
    ~BaseExplorerPage() override;

    bool eventFilter(QObject *obj, QEvent *ev) override;
    void resizeEvent(QResizeEvent* event) override;

    Ui::ExplorerPage *ui;
    bool initialPaintDone;
    bool selectAllStatus;
    QDir currentDir;
    std::vector<BaseFileEntryWidget*> entryWidgets;

    void SetAndTrimCurDirLabel();

private slots:
    void on_SelectAllButton_clicked();
    void on_homeButton_clicked();

private:
    virtual void Populate() = 0;
    void HomeButtonLongPressReleased();
    void HomeButtonLongPressAction();

    QTimer homeLongPressTimer;
    QString curDirSerializedAdress;
};

class VocabExplorerPage : public BaseExplorerPage
{
    Q_OBJECT

public:
    VocabExplorerPage(QWidget *parent = nullptr);
    ~VocabExplorerPage() override;

    void Populate() override;

private:
    void DeleteEntryWidgets();
};

class NoteExplorerPage : public BaseExplorerPage
{
    Q_OBJECT

public:
    NoteExplorerPage(QWidget *parent = nullptr);
    ~NoteExplorerPage() override;

    void Populate() override;

private:
    void DeleteEntryWidgets();
};

#endif // EXPLORERPAGE_H
