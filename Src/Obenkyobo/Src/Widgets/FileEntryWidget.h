#ifndef FileEntryWidget_H
#define FileEntryWidget_H

#include <QWidget>
#include <QSettings>
#include <QFileInfo>

namespace Ui
{
    class BaseFileEntryWidget;
    class BaseVocabFileEntryWidget;
    class VocabFileEntryWidget;
    class VocabFileUpDirWidget;
    class NoteFileEntryWidget;
    class NoteFileUpWidget;
}

class BaseFileEntryWidget : public QWidget
{
    Q_OBJECT

public :
    QFileInfo const& FileInfo() { return fileInfo; }
    virtual void OnScrollbarToggled() = 0;
    virtual void FakeClick(bool /*checked*/) {};

protected :
    BaseFileEntryWidget(QWidget *parent = nullptr);
    ~BaseFileEntryWidget() = default;

    virtual void ForceTitleButtonSize() = 0;

    QFileInfo fileInfo;

private slots:
    virtual void on_TitleButton_clicked() = 0;
};

/********************** VOCAB FILES **********************/

class BaseVocabFileEntryWidget : public BaseFileEntryWidget
{
    Q_OBJECT

public :
    virtual void SetLearningScoreText(QString learningScoreText);

protected :
    BaseVocabFileEntryWidget(QWidget *parent = nullptr);
    ~BaseVocabFileEntryWidget();

    void ForceTitleButtonSize() override;

    Ui::BaseVocabFileEntryWidget *ui;

private slots:
    virtual void on_checkBox_clicked(bool checked);
};

class VocabFileEntryWidget : public BaseVocabFileEntryWidget
{
    Q_OBJECT

public:
    VocabFileEntryWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    ~VocabFileEntryWidget() = default;

    void SetLearningScoreText(QString learningScoreText) override;
    void OnScrollbarToggled() override;
    void FakeClick(bool checked) override;

private slots:
    void on_TitleButton_clicked() override;
    void on_checkBox_clicked(bool checked) override;

private:
    void SetAndTrimCurDirLabel();
};

class VocabFileUpDirWidget : public BaseVocabFileEntryWidget
{
    Q_OBJECT

public:
    VocabFileUpDirWidget(QFileInfo fileInfo, QWidget *parent = nullptr);
    ~VocabFileUpDirWidget() = default;

    void OnScrollbarToggled() override;

private slots:
    void on_TitleButton_clicked() override;
};

/********************** NOTES FILES **********************/

class BaseNoteFileEntryWidget : public BaseFileEntryWidget
{
    Q_OBJECT

protected :
    BaseNoteFileEntryWidget(QWidget */*parent*/ = nullptr) {};
    ~BaseNoteFileEntryWidget() {};

    void ForceTitleButtonSize() override {};
};

class NoteFileEntryWidget : public BaseNoteFileEntryWidget
{
    Q_OBJECT

public:
    NoteFileEntryWidget(QFileInfo /*fileInfo*/, QWidget */*parent*/ = nullptr) {};
    ~NoteFileEntryWidget() = default;

    void OnScrollbarToggled() override {}

private slots:
    void on_TitleButton_clicked() override {};
};

class NoteFileUpDirWidget : public BaseNoteFileEntryWidget
{
    Q_OBJECT

public:
    NoteFileUpDirWidget(QFileInfo /*fileInfo*/, QWidget */*parent*/ = nullptr) {};
    ~NoteFileUpDirWidget() = default;

    void OnScrollbarToggled() override {}

private slots:
    void on_TitleButton_clicked() override {};
};

#endif // FileEntryWidget_H
