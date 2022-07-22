#ifndef STATISTICSPAGE_H
#define STATISTICSPAGE_H

#include <QWidget>

namespace Ui
{
    class StatisticsPage;
}

class StatisticsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsPage(QWidget *parent = nullptr);
    ~StatisticsPage();

    void UpdateScoreCounters() const;

private slots:
    void on_ResetPopups_clicked();
    void on_ResetStatsButton_clicked();
    void on_ResetKanasWeights_clicked();
    void on_DisableVocabSheets_clicked();
    void on_ResetVocabSheets_clicked();

private:
    void ParseConfigFile();

    Ui::StatisticsPage *ui;
};

#endif // STATISTICSPAGE_H
