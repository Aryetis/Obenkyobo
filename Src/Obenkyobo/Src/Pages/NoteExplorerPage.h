#ifndef NOTEEXPLORERPAGE_H
#define NOTEEXPLORERPAGE_H
#include <QWidget>

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

private:
    Ui::NoteExplorerPage *ui;
};

#endif // NOTEEXPLORERPAGE_H
