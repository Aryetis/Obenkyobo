#include "Src/Widgets/VocabFileEntryWidget.h"
#include "ui_VocabFileEntryWidget.h"
#include "Src/mainwindow.h"
#include "Src/Pages/VocabularyDisplayPage.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/Pages/VocabExplorerPage.h"

#include "Src/GetMy.h"
#include "Src/tools.h"


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

void VocabFileEntryWidget::on_checkBox_clicked(bool /*checked*/)
{
    // TODO
}

void VocabFileEntryWidget::FakeClick(bool checked)
{
    ui->checkBox->setChecked(checked);
}
