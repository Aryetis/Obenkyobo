#ifndef SCREENSETTINGS_H
#define SCREENSETTINGS_H

#include <QWidget>
#include "koboplatformfunctions.h"

namespace Ui
{
    class ScreenSettings;
}

class ScreenSettings : public QWidget
{
    Q_OBJECT

public:
    explicit             ScreenSettings(QWidget *parent = nullptr);
    ~ScreenSettings()    override;

private slots:
    void                 on_TintSlider_valueChanged(int value);

    void                 on_LuminositySlider_valueChanged(int value);

private:
    Ui::ScreenSettings*  ui;
    int                  luminosity;
    int                  tint;
    bool                 noSettingAvailable;
    KoboDeviceDescriptor desc;
};

#endif // SCREENSETTINGS_H
