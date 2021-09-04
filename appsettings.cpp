#include "appsettings.h"
#include "ui_appsettings.h"
#include "SettingsSerializer.h"
#include "GetMy.h"

AppSettings::AppSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppSettings)
{
    ui->setupUi(this);

    GetMy::GetInstance().SetAppSettingWidget(this);
}

void AppSettings::InitializeAppSettings()
{
    int appStatisticsScore = SettingsSerializer::settings.value("AppStatistics/score", 0).toInt();
    ui->ScoreCounterValueLabel->setText(QString::number(appStatisticsScore));
    int appStatisticsError = SettingsSerializer::settings.value("AppStatistics/error", 0).toInt();
    ui->ErrorsCounterValueLabel->setText(QString::number(appStatisticsError));

    nbrOfEntryLinesIdx = SettingsSerializer::settings.value("AppStatistics/entriesPerLine", 0).toInt();
    ui->nbrOfEntryLinesDropdown->setCurrentIndex(nbrOfEntryLinesIdx);
}

AppSettings::~AppSettings()
{
    delete ui;
}

void AppSettings::on_ResetStatsButton_clicked()
{
    SettingsSerializer::settings.setValue("AppStatistics/score", 0);
    SettingsSerializer::settings.setValue("AppStatistics/error", 0);

    ui->ScoreCounterValueLabel->setText("0");
    ui->ErrorsCounterValueLabel->setText("0");
}

void AppSettings::on_nbrOfEntryLinesDropdown_activated(int index)
{
    nbrOfEntryLinesIdx = index;
    SettingsSerializer::settings.setValue("AppStatistics/entriesPerLine", index);
}
