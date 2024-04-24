#ifndef VOCABBASEENTRYWIDGET_H
#define VOCABBASEENTRYWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QFileInfo>

namespace Ui
{
    class VocabBaseEntryWidget;
    class VocabFileEntryWidget;
    class VocabFileUpDirWidget;
}

class VocabBaseEntryWidget : public QWidget
{
    Q_OBJECT
public :
    explicit VocabBaseEntryWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    VocabBaseEntryWidget(QWidget *parent = nullptr);
    ~VocabBaseEntryWidget();

    QFileInfo const& VocabFileInfo() { return vocabFileInfo; }
    virtual void SetLearningScoreText(QString learningScoreText);
    virtual void OnScrollbarEnabled() = 0;
    virtual void FakeClick(bool checked);

private slots:
    virtual void on_TitleButton_clicked() = 0;
    virtual void on_checkBox_clicked(bool checked);

protected :
    virtual void resizeEvent(QResizeEvent* event) override = 0;
    void ForceTitleButtonSize();

    Ui::VocabBaseEntryWidget *ui;
    QFileInfo vocabFileInfo;
    bool initialPaintDone;
    bool scrollBarDisplayed;
};

class VocabFileEntryWidget : public VocabBaseEntryWidget
{
    Q_OBJECT

public:
    explicit VocabFileEntryWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    VocabFileEntryWidget(QWidget *parent = nullptr);
    ~VocabFileEntryWidget() = default;

    void SetLearningScoreText(QString learningScoreText) override;
    void OnScrollbarEnabled() override;
    void FakeClick(bool checked) override;

private slots:
    void on_TitleButton_clicked() override;
    void on_checkBox_clicked(bool checked) override;

protected :
    void resizeEvent(QResizeEvent* event) override;

private:
    void SetAndTrimCurDirLabel();
};


class VocabUpDirWidget : public VocabBaseEntryWidget
{
    Q_OBJECT

public:
    VocabUpDirWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    ~VocabUpDirWidget() = default;

    void OnScrollbarEnabled() override;

private slots:
    void on_TitleButton_clicked() override;

protected :
    void resizeEvent(QResizeEvent* event) override;
};

#endif // VOCABBASEENTRYWIDGET_H
