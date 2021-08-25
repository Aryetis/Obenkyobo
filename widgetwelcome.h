#ifndef WIDGETWELCOME_H
#define WIDGETWELCOME_H

#include <QWidget>

namespace Ui {
class WidgetWelcome;
}

class WidgetWelcome : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetWelcome(QWidget *parent = nullptr);
    ~WidgetWelcome();

private:
    Ui::WidgetWelcome *ui;
};

#endif // WIDGETWELCOME_H
