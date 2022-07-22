#include "StatisticsPage.h"
#include "ui_StatisticsPage.h"
#include "Src/KanasTables.h"
#include "Src/VocabularyParser.h"
#include "Src/GetMy.h"

StatisticsPage::StatisticsPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::StatisticsPage)
{
    ui->setupUi(this);

    ParseConfigFile();

    GetMy::Instance().SetStatisticsInst(this);
}

StatisticsPage::~StatisticsPage()
{
    delete ui;
}


void StatisticsPage::UpdateScoreCounters() const
{
    int appStatisticsScore = GetMy::Instance().SettingSerializerInst()->value("AppStatistics/score", 0).toInt();
    ui->ScoreCounterValueLabel->setText(QString::number(appStatisticsScore));
    int appStatisticsError = GetMy::Instance().SettingSerializerInst()->value("AppStatistics/error", 0).toInt();
    ui->ErrorsCounterValueLabel->setText(QString::number(appStatisticsError));
}

void StatisticsPage::on_ResetPopups_clicked()
{
    GetMy::Instance().SettingSerializerInst()->setValue("AppSettings/firstTimeMainWindowPage", true);
    GetMy::Instance().SettingSerializerInst()->setValue("AppSettings/firstTimeVocabListPage", true);
    GetMy::Instance().SettingSerializerInst()->setValue("AppSettings/firstTimeVocabDisplayPage", true);
    GetMy::Instance().SettingSerializerInst()->setValue("AppSettings/firstTimeKanasEditPage", true);
}

void StatisticsPage::on_ResetStatsButton_clicked()
{
    GetMy::Instance().SettingSerializerInst()->setValue("AppStatistics/score", 0);
    GetMy::Instance().SettingSerializerInst()->setValue("AppStatistics/error", 0);

    ui->ScoreCounterValueLabel->setText("0");
    ui->ErrorsCounterValueLabel->setText("0");
}

void StatisticsPage::on_ResetKanasWeights_clicked()
{
    KanasTables::HiraganaSymbolsTableFamily.ResetWeights();
    KanasTables::KatakanaSymbolsTableFamily.ResetWeights();
}

void StatisticsPage::on_DisableVocabSheets_clicked()
{
    GetMy::Instance().ClearEnabledVocabSheets();
}

void StatisticsPage::on_ResetVocabSheets_clicked()
{
    QSet<QString>::iterator it = GetMy::Instance().ModifyEnabledVocabSheets().begin();
    while (it != GetMy::Instance().GetEnabledVocabSheets().end())
        VocabDataFile::ResetLearningScore(*it++);
}

void StatisticsPage::ParseConfigFile()
{
    QSet<QString> parsedEnabledSheets = GetMy::Instance().SettingSerializerInst()->value("Vocabs/enabledSheets").value<QSet<QString>>();
    QMutableSetIterator<QString> it(parsedEnabledSheets);
    while (it.hasNext()) // removing dead sheets ...
    {
        QString sheetPath = it.next();
        if (!QFile::exists(sheetPath))
            it.remove();
    }
    GetMy::Instance().AddEnabledVocabSheets(parsedEnabledSheets);
}
