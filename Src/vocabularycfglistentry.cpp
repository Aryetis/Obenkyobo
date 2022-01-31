#include "vocabularycfglistentry.h"
#include "ui_vocabularycfglistentry.h"

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

    ui->TitleButton->setText(fi.fileName());
}

VocabularyCfgListEntry::~VocabularyCfgListEntry()
{
    delete ui;
}

void VocabularyCfgListEntry::on_TitleButton_clicked()
{

}
