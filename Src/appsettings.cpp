#include "Src/appsettings.h"
#include "ui_appsettings.h"
#include "Src/GetMy.h"
#include "Src/tools.h"
#include "Src/mainwindow.h"

#include <QAbstractItemView>

AppSettings::AppSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppSettings)
{
    ui->setupUi(this);

    settingsSerializer = GetMy::Instance().SettingSerializer();
    nbrOfEntryLinesIdx = settingsSerializer->value("AppSettings/entriesPerLineIdx", 2).toInt();
    randomChoiceIdx = settingsSerializer->value("AppSettings/randomChoiceIdx", 1).toInt();
    hardRefreshFreqIdx = settingsSerializer->value("AppSettings/hardRefreshFreqIdx", 3).toInt();
    batteryFormatIdx = settingsSerializer->value("AppSettings/batteryFormatIdx", 0).toInt();
    dateFormatIdx = settingsSerializer->value("AppSettings/entriesPerLineIdx", (Tools::IsLocalTimeFormatUS()) ? 1 : 0).toInt();

#ifdef QT_NO_DEBUG
    keepWifiOn = settingsSerializer->value("AppSettings/wifi", 0).toBool();
#else
    keepWifiOn = true;
#endif

    if (!keepWifiOn)
        KoboPlatformFunctions::disableWiFiConnection();

    GetMy::Instance().SetAppSettingWidget(this);
}


void AppSettings::resizeEvent(QResizeEvent* event)
{
   QWidget::resizeEvent(event);

   ui->WifiCheckBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                                                    .arg(ui->WifiLabel->height()));
}

void AppSettings::InitializeUIValues() const
{
    int appStatisticsScore = settingsSerializer->value("AppStatistics/score", 0).toInt();
    ui->ScoreCounterValueLabel->setText(QString::number(appStatisticsScore));
    int appStatisticsError = settingsSerializer->value("AppStatistics/error", 0).toInt();
    ui->ErrorsCounterValueLabel->setText(QString::number(appStatisticsError));
    ui->WifiCheckBox->setChecked(keepWifiOn);

    ui->nbrOfEntryLinesDropdown->setCurrentIndex(nbrOfEntryLinesIdx);
    ui->RandomnessDropdown->setCurrentIndex(randomChoiceIdx);
    ui->HardRefreshDropdown->setCurrentIndex(hardRefreshFreqIdx);
    ui->BatteryIndicatorDropdown->setCurrentIndex(batteryFormatIdx);
    ui->DateDisplayFormatDropdown->setCurrentIndex(dateFormatIdx);
}

AppSettings::~AppSettings()
{
    delete ui;
}

bool AppSettings::IsThereEnough(QcmExercice::QcmExerciceType qcmType) const
{
    int minRequiredSymbol = GetMy::Instance().AppSettingWidget().GetNumberOfEntryLine() *
                            GetMy::Instance().AppSettingWidget().GetNumberOfEntryRow();

    switch (qcmType)
    {
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Hiragana_to_Romanji_Kbd :
        case QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM :
        {
            return (SymbolsTables::HiraganaSymbolsTableFamily.NbrOfEnabled() >= minRequiredSymbol);
        }
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM :
        case QcmExercice::QcmExerciceType::Katakana_to_Romanji_Kbd :
        case QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM :
        {
            return (SymbolsTables::KatakanaSymbolsTableFamily.NbrOfEnabled() >= minRequiredSymbol);
        }
    }

    assert(false);
}

void AppSettings::on_ResetStatsButton_clicked()
{
    settingsSerializer->setValue("AppStatistics/score", 0);
    settingsSerializer->setValue("AppStatistics/error", 0);

    ui->ScoreCounterValueLabel->setText("0");
    ui->ErrorsCounterValueLabel->setText("0");
}

void AppSettings::on_nbrOfEntryLinesDropdown_activated(int index)
{
    nbrOfEntryLinesIdx = index;
    settingsSerializer->setValue("AppSettings/entriesPerLineIdx", index);
}

void AppSettings::on_RandomnessDropdown_currentIndexChanged(int index)
{
    randomChoiceIdx = index;
    settingsSerializer->setValue("AppSettings/randomChoiceIdx", index);
}

bool AppSettings::IsWeightedRandomEnabled() const
{
    return randomChoiceIdx == 1;
}

// TODO now fix
void AppSettings::on_WifiCheckBox_clicked(bool /*checked*/)
{
//    if (checked != wifi)
//    {
//        wifi = checked;
//        settingsSerializer->value("AppSettings/wifi", wifi).toBool();
//        if (wifi)
//            KoboPlatformFunctions::enableWiFiConnection();
//        else
//            KoboPlatformFunctions::disableWiFiConnection();
//    }
}

void AppSettings::on_ResetWeightsButton_clicked()
{
    SymbolsTables::HiraganaSymbolsTableFamily.ResetWeights();
    SymbolsTables::KatakanaSymbolsTableFamily.ResetWeights();
}

void AppSettings::on_HardRefreshDropdown_currentIndexChanged(int index)
{
    hardRefreshFreqIdx = index;
    settingsSerializer->setValue("AppSettings/hardRefreshFreqIdx", index);
}

void AppSettings::on_BatteryIndicatorDropdown_currentIndexChanged(int index)
{
    batteryFormatIdx = index;
    settingsSerializer->setValue("AppSettings/batteryFormatIdx", index);
    GetMy::Instance().MainWindowWidget().UpdateStatusBarGeometry();
}

void AppSettings::on_DateDisplayFormatDropdown_currentIndexChanged(int index)
{
    dateFormatIdx = index;
    settingsSerializer->setValue("AppSettings/entriesPerLineIdx", index);
    GetMy::Instance().MainWindowWidget().UpdateStatusBarGeometry();
}


int AppSettings::HardRefreshFreq() const
{
    bool parsed;
    int num = ui->HardRefreshDropdown->currentText().toInt(&parsed);

    return (parsed) ? num : -1;
}