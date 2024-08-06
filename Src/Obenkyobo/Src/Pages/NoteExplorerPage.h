#ifndef NOTEEXPLORERPAGE_H
#define NOTEEXPLORERPAGE_H
#include <QWidget>
#include <QDir>
#include <QTimer>

namespace Ui
{
    class NoteExplorerPage;
}

class NoteExplorerPage : public QWidget
{
    Q_OBJECT

public:
    explicit NoteExplorerPage(QWidget *parent = nullptr);
    ~NoteExplorerPage() override;

    void InitializeNoteExplorerPage();

    QDir currentDir;

private:
    void HomeButtonLongPressReleased();
    void HomeButtonLongPressAction();

    Ui::NoteExplorerPage *ui;

    QTimer homeLongPressTimer;
};

#endif // NOTEEXPLORERPAGE_H
