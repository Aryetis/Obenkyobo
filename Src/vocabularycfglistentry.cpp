#include "vocabularycfglistentry.h"
#include "ui_vocabularycfglistentry.h"
#include "mainwindow.h"
#include "Src/vocabularydisplay.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/vocabularylearneditset.h"

#include "GetMy.h"
#include "tools.h"


VocabularyCfgListEntry::VocabularyCfgListEntry(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyCfgListEntry)
{
    ui->setupUi(this);
}

VocabularyCfgListEntry::VocabularyCfgListEntry(QFileInfo fi, bool dirtyUpDirHack, QWidget *parent)
    : QWidget(parent), ui(new Ui::VocabularyCfgListEntry)
    , vocabFileInfo(fi), vocabSetting(fi.filePath(), QSettings::IniFormat)
{
    ui->setupUi(this);

    QString title;
    if (dirtyUpDirHack)
        title = "[UP_DIR] ..";
    else
    {
        title = vocabFileInfo.completeBaseName();
        if (fi.isDir())
            title = "[DIR] " + vocabFileInfo.fileName(); // because hidden dir completeBaseName is empty
        if (title.size() > 20)
        {
            title.truncate(20);
            title.append("...");
        }
    }
    ui->TitleButton->setText(title);
    // another dirty hack because koboQT... for some reasons I can't use TitleButton.height to set checkbox's one
    ui->checkBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                                 .arg(GetMy::Instance().Descriptor().height/20) );
    setMaximumHeight(GetMy::Instance().Descriptor().height/20);
}

VocabularyCfgListEntry::~VocabularyCfgListEntry()
{
    delete ui;
}

void VocabularyCfgListEntry::on_TitleButton_clicked()
{
    if (vocabFileInfo.isDir())
    {
        GetMy::Instance().VocabularyLearnEditSetWidget()->Populate(vocabFileInfo.filePath());
    }
    else
    {
        GetMy::Instance().VocabularyDisplayWidget()->InitializeGrid(this);
        GetMy::Instance().MainWindowWidget().SwitchStackedWidgetIndex(8);

        if ( GetMy::Instance().AppSettingWidget().GetSettingsSerializer()->value("AppSettings/firstTimeVocabDisplayPage", true).toBool() )
        {
            Tools::GetInstance().DisplayPopup("You can click the top row buttons to hide/show the associated column.\n"
                                              "Each cell is also independently clickable.\n"
                                              "Combine that with the \"Randomize\" button to enhance your learning session");
            GetMy::Instance().AppSettingWidget().GetSettingsSerializer()->setValue("AppSettings/firstTimeVocabDisplayPage", false);
        }
    }
}

void VocabularyCfgListEntry::on_checkBox_clicked(bool /*checked*/)
{
    // TODO
}

void VocabularyCfgListEntry::FakeClick(bool checked)
{
    ui->checkBox->setChecked(checked);
}
