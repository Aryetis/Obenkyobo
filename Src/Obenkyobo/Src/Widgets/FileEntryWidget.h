#ifndef FileEntryWidget_H
#define FileEntryWidget_H

#include <QWidget>
#include <QSettings>
#include <QFileInfo>
#include <QPushButton>

namespace Ui
{
    class BaseFileEntryWidget;
    class BaseVocabFileEntryWidget;
    class VocabFileEntryWidget;
    class VocabFileUpDirWidget;
    class NoteFileEntryWidget;
    class NoteFileUpWidget;
}

class BaseFileEntryWidget
{
public:
    QFileInfo const& FileInfo() const { return fileInfo; }
    virtual void FakeClick(bool /*checked*/) {};

    void OnScrollbarToggled();

protected:
    BaseFileEntryWidget() = default;
    ~BaseFileEntryWidget() = default;

    virtual void ForceTitleButtonSize() = 0;
    void ForceTitleButtonSize(QPushButton* target);
    void BaseSetAndTrimCurDirLabel(QPushButton* target);

    QFileInfo fileInfo;

private:
    virtual float EntryWidth() = 0;
    virtual bool IsScrollBarDisplayed() = 0;
    virtual void SetAndTrimCurDirLabel() = 0;
};

/********************** VOCAB FILES **********************/

class BaseVocabFileEntryWidget : public QWidget, public BaseFileEntryWidget
{
    Q_OBJECT

public:
    virtual void SetLearningScoreText(QString learningScoreText);

protected:
    BaseVocabFileEntryWidget(QWidget *parent = nullptr);
    ~BaseVocabFileEntryWidget();

    void ForceTitleButtonSize() override;

    Ui::BaseVocabFileEntryWidget *ui;

private:
    float EntryWidth() override;
    bool IsScrollBarDisplayed() override;

private slots:
    virtual void on_TitleButton_clicked() = 0;
    virtual void on_checkBox_clicked(bool checked);
};

class VocabFileEntryWidget : public BaseVocabFileEntryWidget
{
    Q_OBJECT

public:
    VocabFileEntryWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    ~VocabFileEntryWidget() = default;

    void SetLearningScoreText(QString learningScoreText) override;
    void FakeClick(bool checked) override;

private slots:
    void on_TitleButton_clicked() override;
    void on_checkBox_clicked(bool checked) override;

private:
    void SetAndTrimCurDirLabel() override;
};

class VocabFileUpDirWidget : public BaseVocabFileEntryWidget
{
    Q_OBJECT

public:
    VocabFileUpDirWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    ~VocabFileUpDirWidget() = default;

private slots:
    void on_TitleButton_clicked() override;

private:
    void SetAndTrimCurDirLabel() override;
};

/********************** NOTES FILES **********************/

class BaseNoteFileEntryWidget : public QPushButton, public BaseFileEntryWidget
{
    Q_OBJECT

public :
    virtual void OnClick() = 0;

protected :
    BaseNoteFileEntryWidget(QWidget *parent = nullptr);
    ~BaseNoteFileEntryWidget();

    void ForceTitleButtonSize() override;

private:
    float EntryWidth() override;
    bool IsScrollBarDisplayed() override;
};

class NoteFileEntryWidget : public BaseNoteFileEntryWidget
{
    Q_OBJECT

public:
    NoteFileEntryWidget(QFileInfo fileInfo_, QWidget *parent = nullptr);
    ~NoteFileEntryWidget() = default;

    void OnClick() override;

private:
    void SetAndTrimCurDirLabel() override;
};

class NoteFileUpDirWidget : public BaseNoteFileEntryWidget
{
    Q_OBJECT

public:
    NoteFileUpDirWidget(QFileInfo fileInfo_, QWidget *parent = nullptr);
    ~NoteFileUpDirWidget() = default;

    void OnClick() override;

private:
    void SetAndTrimCurDirLabel() override;
};

#endif // FileEntryWidget_H
