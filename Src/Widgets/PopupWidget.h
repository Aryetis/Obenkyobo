#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QDialog>

namespace Ui {
class Popup;
}

class PopupWidget : public QDialog
{
    Q_OBJECT

public:
    PopupWidget(QString message, bool fullscreen, QWidget *parent = nullptr);
    ~PopupWidget();

private slots:
    void on_OkButton_clicked();

private:
    Ui::Popup *ui;
};

#endif // POPUPWIDGET_H
