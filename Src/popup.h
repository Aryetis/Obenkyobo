#ifndef POPUP_H
#define POPUP_H

#include <QDialog>

namespace Ui {
class Popup;
}

class Popup : public QDialog
{
    Q_OBJECT

public:
    Popup(QString message, bool fullscreen, QWidget *parent = nullptr);
    ~Popup();

private slots:
    void on_OkButton_clicked();

private:
    Ui::Popup *ui;
};

#endif // POPUP_H
