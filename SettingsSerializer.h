#ifndef SETTINGSSERIALIZER_H
#define SETTINGSSERIALIZER_H
#include <QSettings>

namespace SettingsSerializer
{
    static QSettings settings(QString("config.cfg"), QSettings::IniFormat);
}
#endif // SETTINGSSERIALIZER_H
