#ifndef NOTEEXPLORERPAGE_H
#define NOTEEXPLORERPAGE_H

#include <QWidget>
#include <QDir>
#include <QTimer>
#include <QPushButton>

namespace Ui
{
    class NoteExplorerPage;
}

class BaseNoteFileEntryWidget;
class NoteExplorerPage : public QWidget
{
    Q_OBJECT

public:
    explicit NoteExplorerPage(QWidget *parent = nullptr);
    ~NoteExplorerPage() override;

    void Populate(QDir dir);
    void OnSleep() const;
    void OnWakeUp() const;

    void InitializeNoteExplorerPage();
    QDir currentDir;

private:
    void HomeButtonLongPressReleased();
    void HomeButtonLongPressAction();
    void Populate(QDir dir);
    void Populate();
    void SetAndTrimCurDirLabel();
    void OnSliderReleased() const;
    void OnValueChanged(int) const;
    void DeleteEntryWidgets();

    Ui::NoteExplorerPage *ui;
    std::vector<BaseNoteFileEntryWidget*> entryWidgets;

    bool initialPaintDone;
    QTimer homeLongPressTimer;
};

#endif // NOTEEXPLORERPAGE_H
