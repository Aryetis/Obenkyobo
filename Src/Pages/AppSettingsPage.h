#ifndef APPSETTINGSPAGE_H
#define APPSETTINGSPAGE_H

#include <QWidget>
#include <QSettings>
#include <QMap>
#include "Src/Pages/QcmExercicePage.h"

#define ENTRY_PER_ROW 3

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
    bool IsThereEnough(QcmExercicePage::QcmExerciceType qcmType) const;
    bool IsWeightedRandomEnabled() const;
    int GetBatteryFormatIdx() const { return batteryFormatIdx; }
    int GetDateFormatIdx() const { return dateFormatIdx; } // 0 <=> 13:37, 1 <=> 1:37 pm // TODO : use bool and IsDateAmPmFormat
    int GetHardRefreshFreq() const;
    bool GetKanaHardRefresh() const { return kanaHardRefresh; }
    int GetVocabFntSize() const;
    void UpdateScoreCounters() const;
    bool GetWifiStatus() const { return wifiStatus; }

    QSet<QString> const& GetEnabledVocabSheets() const { return enabledSheets; }
    void AddEnabledVocabSheet(QString filePath);
    void AddEnabledVocabSheets(QSet<QString> filePaths);
    void RemoveEnabledVocabSheet(QString filePath);
    void RemoveEnabledVocabSheets(QSet<QString> filePaths);

    QSettings* GetSettingsSerializer() const { return settingsSerializer; }

private:
    void ParseConfigFile();
    void InitializeUIValues() const;

private slots:
    void on_ResetStatsButton_clicked();
    void on_nbrOfEntryLinesDropdown_activated(int index);
    void on_RandomnessDropdown_currentIndexChanged(int index);
    void on_WifiCheckBox_clicked(bool checked);
    void on_ResetWeightsButton_clicked();
    void on_HardRefreshDropdown_currentIndexChanged(int index);
    void on_BatteryIndicatorDropdown_currentIndexChanged(int index);
    void on_DateDisplayFormatDropdown_currentIndexChanged(int index);
    void on_RowPerPageComboBox_currentIndexChanged(int index);
    void on_KanaHardRefreshCheckBox_clicked(bool checked);
    void on_comboBox_currentIndexChanged(int index);
    void on_resetPopup_clicked();

    void on_DisableVocabSheets_clicked();

private:
    Ui::AppSettingsPage *ui;
    int nbrOfEntryLinesIdx;
    int randomChoiceIdx;
    bool wifiStatus;
    int hardRefreshFreqIdx;
    int batteryFormatIdx;
    int dateFormatIdx;
    int nbrOfRowPerVocabIdx;
    bool kanaHardRefresh;
    int vocabFntSizeIdx;
    QSet<QString> enabledSheets;

    QSettings* settingsSerializer;
};

#endif // APPSETTINGSPAGE_H
