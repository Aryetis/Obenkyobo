#ifndef APPSETTINGSPAGE_H
#define APPSETTINGSPAGE_H

#include <QWidget>
#include <QSettings>
#include <QString>
#include <QSet>
#include "Src/DefinesLand.h"

Q_DECLARE_METATYPE(QSet<QString>)

namespace Ui
{
    class AppSettingsPage;
}

class AppSettingsPage : public QWidget
{
    Q_OBJECT

public:

    explicit AppSettingsPage(QWidget *parent = nullptr);
    ~AppSettingsPage() override;

    int GetNumberOfEntryLine() const { return nbrOfEntryLinesIdx+1; }
    int GetNumberOfEntryRow() const { return ENTRY_PER_ROW; }
    int GetNumberOfEntry() const { return (nbrOfEntryLinesIdx+1) * ENTRY_PER_ROW; }
    int GetNumberOfRowPerVocabPage() const;
    bool IsWeightedRandomEnabled() const;
    int GetBatteryFormatIdx() const { return batteryFormatIdx; }
    int GetHardRefreshFreq() const;
    bool GetKanaHardRefresh() const { return kanaHardRefresh; }
    int GetVocabFntSize() const;
    bool GetWifiStatus() const { return wifiStatus; }
    DisplayLSEnum GetDisplayLSSetting() const { return displayLS; }
    int GetSleepTimerMins() const;
    bool IsDateAmPmFormat() const { return dateFormatIdx == 1; }
    QString GetVocabExplorerHomePath() const { return vocabExplorerHomePath; }
    void SetVocabExplorerHomePath(QString path);
    QString GetNoteExplorerHomePath() const { return noteExplorerHomePath; }
    void SetNoteExplorerHomePath(QString path);
    ScreenSaverSetting GetScreenSaverSetting() const { return static_cast<ScreenSaverSetting>(screenSaverIdx); }

    QSettings* GetSettingsSerializer() const { return settingsSerializer; }

private:
    void ParseConfigFile();
    void InitializeUIValues() const;

private slots:
    void on_nbrOfEntryLinesDropdown_activated(int index);
    void on_RandomnessDropdown_currentIndexChanged(int index);
    void on_WifiCheckBox_clicked(bool checked);
    void on_HardRefreshDropdown_currentIndexChanged(int index);
    void on_BatteryIndicatorDropdown_currentIndexChanged(int index);
    void on_DateDisplayFormatDropdown_currentIndexChanged(int index);
    void on_RowPerPageComboBox_currentIndexChanged(int index);
    void on_KanaHardRefreshCheckBox_clicked(bool checked);
    void on_VocabFntSizeCombox_currentIndexChanged(int index);
    void on_DisplayLSDropdown_currentIndexChanged(int index);
    void on_ScreenSaverDropdown_currentIndexChanged(int index);

    void on_SleepTimerDropdown_currentIndexChanged(int index);

private:
    Ui::AppSettingsPage *ui;
    int nbrOfEntryLinesIdx;
    int randomChoiceIdx;
    bool wifiStatus;
    int hardRefreshFreqIdx;
    int batteryFormatIdx;
    int dateFormatIdx;
    int screenSaverIdx;
    int sleepTimerIdx;
    QString vocabExplorerHomePath;
    QString noteExplorerHomePath;
    int nbrOfRowPerVocabIdx;
    bool kanaHardRefresh;
    int vocabFntSizeIdx;
    DisplayLSEnum displayLS;

    QSettings* settingsSerializer;
};

#endif // APPSETTINGSPAGE_H
