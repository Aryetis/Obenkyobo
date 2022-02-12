#ifndef SCREENSETTINGS_H
#define SCREENSETTINGS_H

#include <QWidget>
#include "kobodevicedescriptor.h"

namespace Ui
{
    class ScreenSettings;
}

class ScreenSettings : public QWidget
{
    Q_OBJECT

public:
    explicit            ScreenSettings(QWidget *parent = nullptr);
    ~ScreenSettings()   override;

    bool                AreSettingsAvailablePopup() const;
    void                OnSleep() const;
    void                OnWakeUp() const;
    void                ToggleLight();

private slots:
    void                 on_TintSlider_valueChanged(int value);
    void                 on_LuminositySlider_valueChanged(int value);

private:
    Ui::ScreenSettings*  ui;
    int                  luminosity;
    int                  tint;
    bool                 settingAvailable;
    bool                 lightToggleStatus;
    const KoboDeviceDescriptor& desc;

    QSettings* settingsSerializer;
};

#endif // SCREENSETTINGS_H
