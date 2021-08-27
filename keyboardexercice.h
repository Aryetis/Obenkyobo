#ifndef KEYBOARDEXERCICE_H
#define KEYBOARDEXERCICE_H

#include <QWidget>

namespace Ui {
class KeyboardExercice;
}

class KeyboardExercice : public QWidget
{
    Q_OBJECT

public:
    explicit KeyboardExercice(QWidget *parent = nullptr);
    ~KeyboardExercice();

private:
    Ui::KeyboardExercice *ui;
};

#endif // KEYBOARDEXERCICE_H
