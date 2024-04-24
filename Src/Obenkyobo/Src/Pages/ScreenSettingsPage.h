#ifndef SCREENSETTINGSPAGE_H
#define SCREENSETTINGSPAGE_H

#include <QWidget>
#include "kobodevicedescriptor.h"

namespace Ui
{
    class ScreenSettingsPage;
}

class ScreenSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit            ScreenSettingsPage(QWidget *parent = nullptr);
    ~ScreenSettingsPage()   override;

    bool                AreSettingsAvailablePopup() const;
    void                OnSleep() const;
    void                OnWakeUp() const;
    void                ToggleLight();

private slots:
    void                 on_TintSlider_valueChanged(int value);
    void                 on_LuminositySlider_valueChanged(int value);

private:
    Ui::ScreenSettingsPage*  ui;
    int                  luminosity;
    int                  tint;
    bool                 settingAvailable;
    bool                 lightToggleStatus;
    const KoboDeviceDescriptor& desc;

    QSettings& settingsSerializer;
};

#endif // SCREENSETTINGSPAGE_H
