#include <QAbstractItemView>
#include "Src/Pages/AppSettingsPage.h"
#include "ui_AppSettingsPage.h"
#include "Src/GetMy.h"
#include "Src/tools.h"
#include "Src/mainwindow.h"
#include "Src/KanasTables.h"

AppSettingsPage::AppSettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppSettingsPage)
{
    ui->setupUi(this);

    ui->WifiCheckBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                                                     .arg((int)(GetMy::Instance().Descriptor().width*0.075f)));
    ui->KanaHardRefreshCheckBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                                                     .arg((int)(GetMy::Instance().Descriptor().width*0.075f)));


    qRegisterMetaTypeStreamOperators<QSet<QString> >("QSet<QString>");
    ParseConfigFile();
    InitializeUIValues();

    GetMy::Instance().SetAppSettingsPageInst(this);
}

void AppSettingsPage::ParseConfigFile()
{
    settingsSerializer = GetMy::Instance().SettingSerializerInst();
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

    enabledSheets = settingsSerializer->value("Vocabs/enabledSheets").value<QSet<QString>>();
    QMutableSetIterator<QString> it(enabledSheets);
    while (it.hasNext()) // removing dead sheets infos
    {
        QString filePath = it.next();
        if (!QFile::exists(filePath)) // warning, can be a directory if I fucked up really bad => don't fully trust what's stored
            it.remove();
    }
}

void AppSettingsPage::InitializeUIValues() const
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

void AppSettingsPage::UpdateScoreCounters() const
{
    int appStatisticsScore = settingsSerializer->value("AppStatistics/score", 0).toInt();
    ui->ScoreCounterValueLabel->setText(QString::number(appStatisticsScore));
    int appStatisticsError = settingsSerializer->value("AppStatistics/error", 0).toInt();
    ui->ErrorsCounterValueLabel->setText(QString::number(appStatisticsError));
}

void AppSettingsPage::AddEnabledVocabSheet(QString filePath)
{
    enabledSheets.insert(filePath);
    settingsSerializer->setValue("Vocabs/enabledSheets", QVariant::fromValue(enabledSheets));
}

void AppSettingsPage::AddEnabledVocabSheets(QSet<QString> filePaths)
{
    enabledSheets = enabledSheets.unite(filePaths);
    settingsSerializer->setValue("Vocabs/enabledSheets", QVariant::fromValue(enabledSheets));
}

void AppSettingsPage::RemoveEnabledVocabSheet(QString filePath)
{
    enabledSheets.remove(filePath);
    settingsSerializer->setValue("Vocabs/enabledSheets", QVariant::fromValue(enabledSheets));
}

void AppSettingsPage::RemoveEnabledVocabSheets(QSet<QString> filePaths)
{
    enabledSheets = enabledSheets.subtract(filePaths);
    settingsSerializer->setValue("Vocabs/enabledSheets", QVariant::fromValue(enabledSheets));
}

AppSettingsPage::~AppSettingsPage()
{
    delete ui;
}

bool AppSettingsPage::IsThereEnough(QcmExerciceType qcmType) const
{
    int minRequiredSymbol = GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryLine() *
                            GetMy::Instance().AppSettingsPageInst().GetNumberOfEntryRow();

    switch (qcmType)
    {
        case QcmExerciceType::Hiragana_to_Romanji_MCQ :
        case QcmExerciceType::Hiragana_to_Romanji_Kbd :
        case QcmExerciceType::Romanji_to_Hiragana_MCQ :
        {
            return (KanasTables::HiraganaSymbolsTableFamily.NbrOfEnabled() >= minRequiredSymbol);
        }
        case QcmExerciceType::Katakana_to_Romanji_MCQ :
        case QcmExerciceType::Katakana_to_Romanji_Kbd :
        case QcmExerciceType::Romanji_to_Katakana_MCQ :
        {
            return (KanasTables::KatakanaSymbolsTableFamily.NbrOfEnabled() >= minRequiredSymbol);
        }
        case QcmExerciceType::Vocabulary_to_Romanji_MCQ :
        case QcmExerciceType::Romanji_to_Vocabulary_MCQ :
        {
            return true; // TODO
        }
    }

    assert(false);
}

void AppSettingsPage::on_ResetStatsButton_clicked()
{
    settingsSerializer->setValue("AppStatistics/score", 0);
    settingsSerializer->setValue("AppStatistics/error", 0);

    ui->ScoreCounterValueLabel->setText("0");
    ui->ErrorsCounterValueLabel->setText("0");
}

void AppSettingsPage::on_nbrOfEntryLinesDropdown_activated(int index)
{
    nbrOfEntryLinesIdx = index;
    settingsSerializer->setValue("AppSettings/entriesPerLineIdx", index);
}

void AppSettingsPage::on_RandomnessDropdown_currentIndexChanged(int index)
{
    randomChoiceIdx = index;
    settingsSerializer->setValue("AppSettings/randomChoiceIdx", index);
}

bool AppSettingsPage::IsWeightedRandomEnabled() const
{
    return randomChoiceIdx == 1;
}

void AppSettingsPage::on_WifiCheckBox_clicked(bool checked)
{
    wifiStatus = checked;
    settingsSerializer->setValue("AppSettings/wifi", wifiStatus);

    // TODO rewrite the whole wifi script... it's a mess
    if (wifiStatus)
        KoboPlatformFunctions::enableWiFiConnection();
    else
        KoboPlatformFunctions::disableWiFiConnection();
}

void AppSettingsPage::on_ResetWeightsButton_clicked()
{
    KanasTables::HiraganaSymbolsTableFamily.ResetWeights();
    KanasTables::KatakanaSymbolsTableFamily.ResetWeights();
}

void AppSettingsPage::on_HardRefreshDropdown_currentIndexChanged(int index)
{
    hardRefreshFreqIdx = index;
    settingsSerializer->setValue("AppSettings/hardRefreshFreqIdx", index);
}

void AppSettingsPage::on_BatteryIndicatorDropdown_currentIndexChanged(int index)
{
    batteryFormatIdx = index;
    settingsSerializer->setValue("AppSettings/batteryFormatIdx", index);
    GetMy::Instance().MainWindowInst().UpdateStatusBarGeometry();
}

void AppSettingsPage::on_DateDisplayFormatDropdown_currentIndexChanged(int index)
{
    dateFormatIdx = index;
    settingsSerializer->setValue("AppSettings/dateFormatIdx", index);
    GetMy::Instance().MainWindowInst().UpdateStatusBarGeometry();
}

void AppSettingsPage::on_RowPerPageComboBox_currentIndexChanged(int index)
{
    nbrOfRowPerVocabIdx = index;
    settingsSerializer->setValue("AppSettings/rowPerVocabPage", index);
}

void AppSettingsPage::on_KanaHardRefreshCheckBox_clicked(bool checked)
{
    kanaHardRefresh = checked;
    settingsSerializer->setValue("AppSettings/kanaHardRefresh", checked);
}

void AppSettingsPage::on_comboBox_currentIndexChanged(int index)
{
    vocabFntSizeIdx = index;
    settingsSerializer->setValue("AppSettings/vocabFntSizeIdx", index);
}

int AppSettingsPage::GetHardRefreshFreq() const
{
    bool parsed;
    int num = ui->HardRefreshDropdown->currentText().toInt(&parsed);

    return (parsed) ? num : -1;
}

int AppSettingsPage::GetNumberOfRowPerVocabPage() const
{
    bool parsed;
    int num = ui->RowPerPageComboBox->currentText().toInt(&parsed);

    return (parsed) ? num : -1;
}

int AppSettingsPage::GetVocabFntSize() const
{
    bool parsed;
    int num = ui->VocabFntSizeCombox->currentText().toInt(&parsed);

    return (parsed) ? num : -1;
}

void AppSettingsPage::on_resetPopup_clicked()
{
    settingsSerializer->setValue("AppSettings/firstTimeMainWindowPage", true);
    settingsSerializer->setValue("AppSettings/firstTimeVocabListPage", true);
    settingsSerializer->setValue("AppSettings/firstTimeVocabDisplayPage", true);
    settingsSerializer->setValue("AppSettings/firstTimeKanasEditPage", true);
}

void AppSettingsPage::on_DisableVocabSheets_clicked()
{
    enabledSheets.clear();
    settingsSerializer->setValue("Vocabs/enabledSheets", QVariant::fromValue(enabledSheets));
}
