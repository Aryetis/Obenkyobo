#ifndef SCREENSETTINGS_H
#define SCREENSETTINGS_H

#include <QWidget>

namespace Ui {
class ScreenSettings;
}

class ScreenSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenSettings(QWidget *parent = nullptr);
    ~ScreenSettings();

private:
    Ui::ScreenSettings *ui;
};

#endif // SCREENSETTINGS_H
