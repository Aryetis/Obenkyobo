#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QWidget>
#include <QSettings>

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

    int GetNumberOfEntryLine() const { return nbrOfEntryLinesIdx+1; }

private slots:
    void on_ResetStatsButton_clicked();
    void on_nbrOfEntryLinesDropdown_activated(int index);

private:
    Ui::AppSettings *ui;
    int nbrOfEntryLinesIdx;

    QSettings* settingsSerializer;
};

#endif // APPSETTINGS_H
