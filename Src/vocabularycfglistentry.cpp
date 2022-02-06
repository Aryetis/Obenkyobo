#include "vocabularycfglistentry.h"
#include "ui_vocabularycfglistentry.h"
#include "mainwindow.h"
#include "Src/vocabularydisplay.h"

#include "GetMy.h"

// TODO popup explaining that you can click every sheet to inspect it, explaining the LS, the checkbox, etc

VocabularyCfgListEntry::VocabularyCfgListEntry(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyCfgListEntry)
{
    ui->setupUi(this);
}

VocabularyCfgListEntry::VocabularyCfgListEntry(QFileInfo fi, QWidget *parent)
    : QWidget(parent), ui(new Ui::VocabularyCfgListEntry)
    , vocabFileInfo(fi), vocabSetting(fi.filePath(), QSettings::IniFormat)
{
    ui->setupUi(this);

    ui->TitleButton->setText(vocabFileInfo.completeBaseName());
    // another dirty hack because koboQT... for some reasons I can't use TitleButton.height to set checkbox's one
    ui->checkBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                                 .arg(GetMy::Instance().Descriptor().height/20) );
}

VocabularyCfgListEntry::~VocabularyCfgListEntry()
{
    delete ui;
}

void VocabularyCfgListEntry::on_TitleButton_clicked()
{
    GetMy::Instance().VocabularyDisplayWidget()->InitializeGrid(this);
    GetMy::Instance().MainWindowWidget().SwitchStackedWidgetIndex(8);
}
