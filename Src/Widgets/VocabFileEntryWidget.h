#ifndef VOCABFILEENTRYWIDGET_H
#define VOCABFILEENTRYWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QFileInfo>

namespace Ui
{
    class VocabFileEntryWidget;
}

class VocabFileEntryWidget : public QWidget
{
    Q_OBJECT

public:
    VocabFileEntryWidget() = delete;
    explicit VocabFileEntryWidget(QFileInfo fileInfo, bool dirtyUpDirHack = false, QWidget *parent = nullptr);
    explicit VocabFileEntryWidget(QWidget *parent = nullptr);
    ~VocabFileEntryWidget();

    QFileInfo const& VocabFileInfo() { return vocabFileInfo; }
    QSettings const& VocabSetting() { return vocabSetting; }
    void SetLearningScoreText(int learningScore);

    void FakeClick(bool checked);

private slots:
    void on_TitleButton_clicked();
    void on_checkBox_clicked(bool checked);

private:
    Ui::VocabFileEntryWidget *ui;
    QFileInfo vocabFileInfo;
    QSettings vocabSetting;
};

#endif // VOCABFILEENTRYWIDGET_H
