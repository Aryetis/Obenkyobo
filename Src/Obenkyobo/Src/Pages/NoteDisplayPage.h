#ifndef NOTEDISPLAYPAGE_H
#define NOTEDISPLAYPAGE_H
#include <QWidget>

namespace Ui
{
    class NoteDisplayPage;
}

class NoteDisplayPage : public QWidget
{
    Q_OBJECT

public:
    explicit NoteDisplayPage(QWidget *parent = nullptr);
    ~NoteDisplayPage() override;

private:
    Ui::NoteDisplayPage *ui;
};

#endif // NOTEDISPLAYPAGE_H
