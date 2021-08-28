#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QWidget>

namespace Ui {
class AppSettings;
}

class AppSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AppSettings(QWidget *parent = nullptr);
    ~AppSettings();

private:
    Ui::AppSettings *ui;
};

#endif // APPSETTINGS_H
