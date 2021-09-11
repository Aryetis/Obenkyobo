#include "appsettings.h"
#include "ui_appsettings.h"
#include "GetMy.h"

AppSettings::AppSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppSettings)
{
    ui->setupUi(this);

    settingsSerializer = GetMy::GetInstance().SettingSerializer();
    nbrOfEntryLinesIdx = settingsSerializer->value("AppSettings/entriesPerLineIdx", 2).toInt();
    randomChoiceIdx = settingsSerializer->value("AppSettings/randomChoiceIdx", 1).toInt();

    InitializeUIValues();

    GetMy::GetInstance().SetAppSettingWidget(this);
}

void AppSettings::InitializeUIValues() const
{
    int appStatisticsScore = settingsSerializer->value("AppStatistics/score", 0).toInt();
    ui->ScoreCounterValueLabel->setText(QString::number(appStatisticsScore));
    int appStatisticsError = settingsSerializer->value("AppStatistics/error", 0).toInt();
    ui->ErrorsCounterValueLabel->setText(QString::number(appStatisticsError));

    ui->nbrOfEntryLinesDropdown->setCurrentIndex(nbrOfEntryLinesIdx);
    ui->RandomnessDropdown->setCurrentIndex(randomChoiceIdx);
}

AppSettings::~AppSettings()
{
    delete ui;
}

bool AppSettings::IsThereEnough(QcmExercice::QcmExerciceType qcmType) const
{
    int minRequiredSymbol = GetMy::GetInstance().AppSettingWidget().GetNumberOfEntryLine() *
                            GetMy::GetInstance().AppSettingWidget().GetNumberOfEntryRow();

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
