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

private slots:
    void on_TintSlider_valueChanged(int value);

    void on_LuminositySlider_valueChanged(int value);

private:
    Ui::ScreenSettings *ui;
};

#endif // SCREENSETTINGS_H
