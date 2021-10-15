#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QWidget>
#include <QSettings>
#include "qcmexercice.h"

#define ENTRY_PER_ROW 3

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
    int GetNumberOfEntryRow() const { return ENTRY_PER_ROW; }
    int GetNumberOfEntry() const { return (nbrOfEntryLinesIdx+1) * ENTRY_PER_ROW; }
    bool IsThereEnough(QcmExercice::QcmExerciceType qcmType) const;
    void InitializeUIValues() const;
    bool IsWeightedRandomEnabled() const;
    bool IsWifiOn() const { return wifi; }
    int getBatteryFormatIdx() const { return batteryFormatIdx; }
    int getDateFormatIdx() const { return dateFormatIdx; } // 0 <=> 13:37, 1 <=> 1:37 pm
    int HardRefreshFreq() const;
protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_ResetStatsButton_clicked();
    void on_nbrOfEntryLinesDropdown_activated(int index);
    void on_RandomnessDropdown_currentIndexChanged(int index);
    void on_WifiCheckBox_clicked(bool checked);
    void on_ResetWeightsButton_clicked();
    void on_HardRefreshDropdown_currentIndexChanged(int index);

    void on_BatteryIndicatorDropdown_currentIndexChanged(int index);

    void on_DateDisplayFormatDropdown_currentIndexChanged(int index);

private:
    Ui::AppSettings *ui;
    int nbrOfEntryLinesIdx;
    int randomChoiceIdx;
    bool wifi;
    int hardRefreshFreqIdx;
    int batteryFormatIdx;
    int dateFormatIdx;

    QSettings* settingsSerializer;
};

#endif // APPSETTINGS_H
