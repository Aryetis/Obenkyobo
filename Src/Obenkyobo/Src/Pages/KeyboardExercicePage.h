#ifndef KEYBOARDEXERCICEPAGE_H
#define KEYBOARDEXERCICEPAGE_H

#include <QWidget>

namespace Ui
{
    class KeyboardExercicePage;
}

class KeyboardExercicePage : public QWidget
{
    Q_OBJECT

public:
    explicit KeyboardExercicePage(QWidget *parent = nullptr);
    ~KeyboardExercicePage();

private:
    Ui::KeyboardExercicePage *ui;
};

#endif // KEYBOARDEXERCICEPAGE_H
