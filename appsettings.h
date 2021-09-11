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


private slots:
    void on_ResetStatsButton_clicked();
    void on_nbrOfEntryLinesDropdown_activated(int index);
    void on_RandomnessDropdown_currentIndexChanged(int index);

private:
    Ui::AppSettings *ui;
    int nbrOfEntryLinesIdx;
    int randomChoiceIdx;

    QSettings* settingsSerializer;
};

#endif // APPSETTINGS_H
