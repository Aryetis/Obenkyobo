#include "Src/Widgets/VocabFileEntryWidget.h"
#include "ui_VocabFileEntryWidget.h"
#include "Src/mainwindow.h"
#include "Src/Pages/VocabularyDisplayPage.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/Pages/VocabExplorerPage.h"

#include "Src/GetMy.h"
#include "Src/tools.h"

#include <QList>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDirIterator>

VocabFileEntryWidget::VocabFileEntryWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabFileEntryWidget)
{
    ui->setupUi(this);
}

VocabFileEntryWidget::VocabFileEntryWidget(QFileInfo fi, bool dirtyUpDirHack, QWidget *parent)
    : QWidget(parent), ui(new Ui::VocabFileEntryWidget)
    , vocabFileInfo(fi), vocabSetting(fi.filePath(), QSettings::IniFormat)
{
    ui->setupUi(this);

    QString title;
    if (dirtyUpDirHack)
    {
        title = "[UP_DIR] ..";
        ui->checkBox->setDisabled(true);
    }
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

        if  (vocabFileInfo.isFile())
        {
            ui->checkBox->setChecked(GetMy::Instance().AppSettingsPageInst().GetEnabledVocabSheets().contains(vocabFileInfo.absoluteFilePath()));
        }
        else
        {
            ui->checkBox->setTristate(true);
            ui->checkBox->setCheckState(Qt::CheckState::Unchecked);

            QDirIterator it(vocabFileInfo.absoluteFilePath(), {"*.oben"}, QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                if (ui->checkBox->checkState() == Qt::CheckState::Unchecked && GetMy::Instance().AppSettingsPageInst().GetEnabledVocabSheets().contains(it.next()))
                {
                    ui->checkBox->setCheckState(Qt::CheckState::Checked);
                }
                else if (ui->checkBox->checkState() == Qt::CheckState::Checked && !GetMy::Instance().AppSettingsPageInst().GetEnabledVocabSheets().contains(it.next()))
                {
                    ui->checkBox->setCheckState(Qt::CheckState::PartiallyChecked);
                    break;
                }
            }
        }
    }
    ui->TitleButton->setText(title);
    // another dirty hack because koboQT... for some reasons I can't use TitleButton.height to set checkbox's one
    ui->checkBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                                 .arg(GetMy::Instance().Descriptor().height/20) );
    setMaximumHeight(GetMy::Instance().Descriptor().height/20);
}

VocabFileEntryWidget::~VocabFileEntryWidget()
{
    delete ui;
}

void VocabFileEntryWidget::on_TitleButton_clicked()
{
    if (vocabFileInfo.isDir())
    {
        GetMy::Instance().VocabExplorerPageInst()->Populate(vocabFileInfo.filePath());
    }
    else
    {
        GetMy::Instance().VocabularyDisplayPageInst()->InitializeGrid(this);
        GetMy::Instance().MainWindowInst().SwitchStackedWidgetIndex(8);

        if ( GetMy::Instance().AppSettingsPageInst().GetSettingsSerializer()->value("AppSettings/firstTimeVocabDisplayPage", true).toBool() )
        {
            Tools::GetInstance().DisplayPopup("You can click the top row buttons to hide/show the associated column.\n"
                                              "Each cell is also independently clickable.\n"
                                              "Combine that with the \"Randomize\" button to enhance your learning session");
            GetMy::Instance().AppSettingsPageInst().GetSettingsSerializer()->setValue("AppSettings/firstTimeVocabDisplayPage", false);
        }
    }
}

Q_DECLARE_METATYPE(QList<int>)
void VocabFileEntryWidget::on_checkBox_clicked(bool checked)
{
    if ( vocabFileInfo.isFile() )
    {
        ui->checkBox->setChecked(checked);
        if (checked)
            GetMy::Instance().AppSettingsPageInst().AddEnabledVocabSheet(vocabFileInfo.absoluteFilePath());
        else
            GetMy::Instance().AppSettingsPageInst().RemoveEnabledVocabSheet(vocabFileInfo.absoluteFilePath());
    }
    else
    {
        ui->checkBox->setCheckState(checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
        QSet<QString> sheetSet;
        QDirIterator it(vocabFileInfo.absoluteFilePath(), {"*.oben"}, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
            sheetSet << it.next();

        if (checked)
            GetMy::Instance().AppSettingsPageInst().AddEnabledVocabSheets(sheetSet);
        else
            GetMy::Instance().AppSettingsPageInst().RemoveEnabledVocabSheets(sheetSet);
    }
}

void VocabFileEntryWidget::FakeClick(bool checked)
{
    ui->checkBox->setChecked(checked);
}
