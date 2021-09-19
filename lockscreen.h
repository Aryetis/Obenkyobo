#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <QWidget>

namespace Ui {
class LockScreen;
}

class LockScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LockScreen(QWidget *parent = nullptr);
    ~LockScreen();

private:
    Ui::LockScreen *ui;
};

#endif // LOCKSCREEN_H
