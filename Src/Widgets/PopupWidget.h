#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QDialog>

namespace Ui
{
    class PopupWidget;
}

class PopupWidget : public QDialog
{
    Q_OBJECT

public:
    PopupWidget() = delete;
    PopupWidget(QString message, bool fullscreen, bool validateButton = true, QWidget *parent = nullptr);
    ~PopupWidget();

private slots:
    void on_OkButton_clicked();

private:
    Ui::PopupWidget *ui;
};

#endif // POPUPWIDGET_H
