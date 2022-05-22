#ifndef VOCABULARYCFGLISTENTRY_H
#define VOCABULARYCFGLISTENTRY_H

#include <QWidget>
#include <QSettings>
#include <QFileInfo>

namespace Ui
{
    class VocabularyCfgListEntry;
}

class VocabularyCfgListEntry : public QWidget
{
    Q_OBJECT

public:
    VocabularyCfgListEntry() = delete;
    explicit VocabularyCfgListEntry(QFileInfo fileInfo, bool dirtyUpDirHack = false, QWidget *parent = nullptr);
    explicit VocabularyCfgListEntry(QWidget *parent = nullptr);
    ~VocabularyCfgListEntry();

    QFileInfo const& VocabFileInfo() { return vocabFileInfo; }
    QSettings const& VocabSetting() { return vocabSetting; }

    void FakeClick(bool checked);

private slots:
    void on_TitleButton_clicked();
    void on_checkBox_clicked(bool checked);

private:
    Ui::VocabularyCfgListEntry *ui;
    QFileInfo vocabFileInfo;
    QSettings vocabSetting;
};

#endif // VOCABULARYCFGLISTENTRY_H
