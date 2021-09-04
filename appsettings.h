#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QWidget>

namespace Ui
{
    class AppSettings;
}

class AppSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AppSettings(QWidget *parent = nullptr);
    ~AppSettings() override;

    void InitializeAppSettings();

private slots:
    void on_ResetStatsButton_clicked();

private:
    Ui::AppSettings *ui;
};

#endif // APPSETTINGS_H
