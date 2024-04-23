#ifndef VOCABFILEENTRYWIDGET_H
#define VOCABFILEENTRYWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QFileInfo>

namespace Ui
{
    class VocabFileEntryWidget;
    class VocabFileUpDirWidget;
}

class VocabFileEntryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VocabFileEntryWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    VocabFileEntryWidget(QWidget *parent = nullptr);
    ~VocabFileEntryWidget();

    QFileInfo const& VocabFileInfo() { return vocabFileInfo; }
    void SetLearningScoreText(QString learningScoreText);

    virtual void OnScrollbarEnabled();

    void FakeClick(bool checked);

private slots:
    virtual void on_TitleButton_clicked();
    virtual void on_checkBox_clicked(bool checked);

protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    void ForceTitleButtonSize();

    Ui::VocabFileEntryWidget *ui;
    QFileInfo vocabFileInfo;
    bool initialPaintDone;
    bool scrollBarDisplayed;

private:
    void SetAndTrimCurDirLabel();
};


class VocabFileUpDirWidget : public VocabFileEntryWidget
{
    Q_OBJECT

public:
    VocabFileUpDirWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    ~VocabFileUpDirWidget();

    void OnScrollbarEnabled() override;

private slots:
    void on_TitleButton_clicked() override;
};

#endif // VOCABFILEENTRYWIDGET_H
