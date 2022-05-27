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

    ui->WifiCheckBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                                                     .arg((int)(GetMy::Instance().Descriptor().width*0.075f)));
    ui->KanaHardRefreshCheckBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                                                     .arg((int)(GetMy::Instance().Descriptor().width*0.075f)));

    ParseConfigFile();
    InitializeUIValues();

    GetMy::Instance().SetAppSettingWidget(this);
}

void AppSettings::ParseConfigFile()
{
    settingsSerializer = GetMy::Instance().SettingSerializer();
    nbrOfEntryLinesIdx = settingsSerializer->value("AppSettings/entriesPerLineIdx", 2).toInt();
    randomChoiceIdx = settingsSerializer->value("AppSettings/randomChoiceIdx", 1).toInt();
    hardRefreshFreqIdx = settingsSerializer->value("AppSettings/hardRefreshFreqIdx", 3).toInt();
    batteryFormatIdx = settingsSerializer->value("AppSettings/batteryFormatIdx", 0).toInt();
    dateFormatIdx = settingsSerializer->value("AppSettings/dateFormatIdx", (Tools::GetInstance().IsLocalTimeFormatUS()) ? 1 : 0).toInt();
    nbrOfRowPerVocabIdx = settingsSerializer->value("AppSettings/rowPerVocabPage", 1).toInt();
    QList<KoboDevice> badScreenList { KoboDevice::KoboTouchAB, KoboDevice::KoboTouchC, KoboDevice::KoboGlo, KoboDevice::KoboMini, KoboDevice::KoboTouch2, KoboDevice::KoboAura, KoboDevice::KoboAuraHD }; // Wild guess ... meh
    kanaHardRefresh = settingsSerializer->value("AppSettings/kanaHardRefresh", badScreenList.contains(GetMy::Instance().Descriptor().device)).toBool();
    vocabFntSizeIdx = settingsSerializer->value("AppSettings/vocabFntSizeIdx", 4).toInt();

#ifdef QT_NO_DEBUG // App doesn't like loosing contact with QtCreator debugger btw
    wifiStatus = settingsSerializer->value("AppSettings/wifi", false).toBool();
#else
    wifiStatus = true;
#endif
    if (wifiStatus)
        KoboPlatformFunctions::enableWiFiConnection();
    else
        KoboPlatformFunctions::disableWiFiConnection();
}

void AppSettings::InitializeUIValues() const
{
    UpdateScoreCounters();
    ui->WifiCheckBox->setChecked(wifiStatus);

    ui->nbrOfEntryLinesDropdown->setCurrentIndex(nbrOfEntryLinesIdx);
    ui->RandomnessDropdown->setCurrentIndex(randomChoiceIdx);
    ui->HardRefreshDropdown->setCurrentIndex(hardRefreshFreqIdx);
    ui->BatteryIndicatorDropdown->blockSignals(true); // preventing UpdateStatusBarGeometry() cause we don't know MainWidget status
    ui->BatteryIndicatorDropdown->setCurrentIndex(batteryFormatIdx);
    ui->BatteryIndicatorDropdown->blockSignals(false);
    ui->DateDisplayFormatDropdown->blockSignals(true); // preventing UpdateStatusBarGeometry() cause we don't know MainWidget status
    ui->DateDisplayFormatDropdown->setCurrentIndex(dateFormatIdx);
    ui->DateDisplayFormatDropdown->blockSignals(false);

    ui->RowPerPageComboBox->setCurrentIndex(nbrOfRowPerVocabIdx);
    ui->KanaHardRefreshCheckBox->setChecked(kanaHardRefresh);
    ui->VocabFntSizeCombox->setCurrentIndex(vocabFntSizeIdx);
}

void AppSettings::UpdateScoreCounters() const
{
    int appStatisticsScore = settingsSerializer->value("AppStatistics/score", 0).toInt();
    ui->ScoreCounterValueLabel->setText(QString::number(appStatisticsScore));
    int appStatisticsError = settingsSerializer->value("AppStatistics/error", 0).toInt();
    ui->ErrorsCounterValueLabel->setText(QString::number(appStatisticsError));
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

void AppSettings::on_WifiCheckBox_clicked(bool checked)
{
    wifiStatus = checked;

    QString originalText = ui->WifiLabel->text();
    ui->WifiLabel->setText(QString(originalText).append(" (Working...)")); // TODO rewrite wifi script completly ...

    wifiStatus = settingsSerializer->value("AppSettings/wifi", checked).toBool();
    if (wifiStatus)
        KoboPlatformFunctions::enableWiFiConnection();
    else
        KoboPlatformFunctions::disableWiFiConnection();

    ui->WifiLabel->setText(originalText);
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
    settingsSerializer->setValue("AppSettings/dateFormatIdx", index);
    GetMy::Instance().MainWindowWidget().UpdateStatusBarGeometry();
}

void AppSettings::on_RowPerPageComboBox_currentIndexChanged(int index)
{
    nbrOfRowPerVocabIdx = index;
    settingsSerializer->setValue("AppSettings/rowPerVocabPage", index);
}

void AppSettings::on_KanaHardRefreshCheckBox_clicked(bool checked)
{
    kanaHardRefresh = checked;
    settingsSerializer->setValue("AppSettings/kanaHardRefresh", checked);
}

void AppSettings::on_comboBox_currentIndexChanged(int index)
{
    vocabFntSizeIdx = index;
    settingsSerializer->setValue("AppSettings/vocabFntSizeIdx", index);
}

int AppSettings::GetNbrOfRowPerVocabPage() const
{
    bool parsed;
    int num = ui->RowPerPageComboBox->currentText().toInt(&parsed);

    return (parsed) ? num : -1;
}

int AppSettings::GetNumberOfRowPerVocabPage() const
{
    bool parsed;
    int num = ui->RowPerPageComboBox->currentText().toInt(&parsed);

    return (parsed) ? num : -1;
}

int AppSettings::GetVocabFntSize() const
{
    bool parsed;
    int num = ui->VocabFntSizeCombox->currentText().toInt(&parsed);

    return (parsed) ? num : -1;
}

void AppSettings::on_resetPopup_clicked()
{
    settingsSerializer->setValue("AppSettings/firstTimeMainWindowPage", true);
    settingsSerializer->setValue("AppSettings/firstTimeVocabListPage", true);
    settingsSerializer->setValue("AppSettings/firstTimeVocabDisplayPage", true);
    settingsSerializer->setValue("AppSettings/firstTimeKanasEditPage", true);
}
