#include "appsettings.h"
#include "ui_appsettings.h"
#include "GetMy.h"

AppSettings::AppSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppSettings)
{
    ui->setupUi(this);

    // Everything below needs to be ready before switching widget => don't delay the initialization
    settingsSerializer = GetMy::GetInstance().SettingSerializer();
    int appStatisticsScore = settingsSerializer->value("AppStatistics/score", 0).toInt();
    ui->ScoreCounterValueLabel->setText(QString::number(appStatisticsScore));
    int appStatisticsError = settingsSerializer->value("AppStatistics/error", 0).toInt();
    ui->ErrorsCounterValueLabel->setText(QString::number(appStatisticsError));

    nbrOfEntryLinesIdx = settingsSerializer->value("AppStatistics/entriesPerLineIdx", 2).toInt();
    ui->nbrOfEntryLinesDropdown->setCurrentIndex(nbrOfEntryLinesIdx);

    GetMy::GetInstance().SetAppSettingWidget(this);
}

AppSettings::~AppSettings()
{
    delete ui;
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
    settingsSerializer->setValue("AppStatistics/entriesPerLineIdx", index);
}
