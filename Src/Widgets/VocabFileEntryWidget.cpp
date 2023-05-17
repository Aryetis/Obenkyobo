#include <QList>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDirIterator>
#include "Src/Widgets/VocabFileEntryWidget.h"
#include "ui_VocabFileEntryWidget.h"
#include "Src/mainwindow.h"
#include "Src/Pages/VocabularyDisplayPage.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/Pages/VocabExplorerPage.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"

VocabFileEntryWidget::VocabFileEntryWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabFileEntryWidget)
{
    ui->setupUi(this);
}

VocabFileEntryWidget::VocabFileEntryWidget(QFileInfo fi, bool dirtyUpDirHack, QWidget *parent)
    : QWidget(parent), ui(new Ui::VocabFileEntryWidget)
    , vocabFileInfo(fi), vocabSetting(fi.filePath(), QSettings::IniFormat), title(), fakeUpDir(dirtyUpDirHack)
{
    ui->setupUi(this);

    if (fakeUpDir)
    {
        title = "[UP_DIR] ..";
        ui->checkBox->setDisabled(true);
    }
    else
    {
        title = vocabFileInfo.completeBaseName();

        if  (vocabFileInfo.isFile())
            ui->checkBox->setChecked(GetMy::Instance().GetEnabledVocabSheets().contains(vocabFileInfo.absoluteFilePath()));
        else
        {
            ui->checkBox->setTristate(true);

            QDirIterator it(vocabFileInfo.absoluteFilePath(), {"*.oben"}, QDir::Files, QDirIterator::Subdirectories);
            bool foundOne = false, missedOne = false;
            while (it.hasNext())
            {
                bool containsItSheet = GetMy::Instance().GetEnabledVocabSheets().contains(it.next());

                foundOne = foundOne || containsItSheet;
                missedOne = missedOne || !containsItSheet;

                if (foundOne && missedOne)
                    break;
            }
            if (foundOne && missedOne)
                ui->checkBox->setCheckState(Qt::CheckState::PartiallyChecked);
            else if (foundOne && !missedOne)
                ui->checkBox->setCheckState(Qt::CheckState::Checked);
            else if (!foundOne && missedOne)
                ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
        }
    }
    // another dirty hack because koboQT... for some reasons I can't use TitleButton.height to set checkbox's one
    ui->checkBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                                 .arg(GetMy::Instance().Descriptor().height/20) );
    setMaximumHeight(GetMy::Instance().Descriptor().height/20);
}

VocabFileEntryWidget::~VocabFileEntryWidget()
{
    delete ui;
}

void VocabFileEntryWidget::SetLearningScoreText(QString learningScoreText)
{
    ui->learningScore->setText(learningScoreText);
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
            GetMy::Instance().ToolsInst()->DisplayPopup("You can click the top row buttons to hide/show each associated column.\n"
                                              "Each cell is also independently clickable.\n"
                                              "Combine those with the \"Randomize\" button to memorize faster.\n"
                                              "\"Reset LS\" will the reset the whole Sheet's Learning Score (LS)");
            GetMy::Instance().AppSettingsPageInst().GetSettingsSerializer()->setValue("AppSettings/firstTimeVocabDisplayPage", false);
        }
    }
}

void VocabFileEntryWidget::on_checkBox_clicked(bool checked)
{
    if ( vocabFileInfo.isFile() )
    {
        ui->checkBox->setChecked(checked);
        if (checked)
            GetMy::Instance().AddEnabledVocabSheet(vocabFileInfo.absoluteFilePath());
        else
            GetMy::Instance().RemoveEnabledVocabSheet(vocabFileInfo.absoluteFilePath());
    }
    else
    {
        ui->checkBox->setCheckState(checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
        QSet<QString> sheetSet;
        QDirIterator it(vocabFileInfo.absoluteFilePath(), {"*.oben"}, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
            sheetSet << it.next();

        if (checked)
            GetMy::Instance().AddEnabledVocabSheets(sheetSet);
        else
            GetMy::Instance().RemoveEnabledVocabSheets(sheetSet);
    }
}

void VocabFileEntryWidget::resizeEvent(QResizeEvent *event)
{
    std::cout << "LOG: VocabExplorerPage::resizeEvent() BEGIN" << std::endl;
    QWidget::resizeEvent(event);

    // ********************* Trim and cut off button's text *********************
    QString prefix { (vocabFileInfo.isDir() && !fakeUpDir) ? "[DIR] " : ""};

    QFontMetricsF fm{ui->TitleButton->font()};
//    int boundingRectFlags = ui->TitleButton != nullptr ? (ui->TitleButton->wordWrap() ? Qt::TextWordWrap : 0) | ui->TitleButton->alignment() : 0;
int boundingRectFlags = 0;
    QRectF newFontSizeRect {fm.boundingRect(ui->TitleButton->rect(), boundingRectFlags, prefix+title)};
    qreal curW {newFontSizeRect.width()};
    if (curW > ui->TitleButton->width()) // if path is too big to fit on screen
    {
        if (vocabFileInfo.isDir() && !fakeUpDir)
            prefix = "[DIR] [...]";
        QString cutTitleLeft = title.left(title.size()/2);
        QString cutTitleRight = title.mid(title.size()/2);

        while (cutTitleLeft.size() != 1)
        {
            if (cutTitleLeft.size() == 0) // no left side => split right into two and loopback
            {
                cutTitleLeft = cutTitleRight.left(cutTitleRight.size()/2);
                cutTitleRight = cutTitleRight.mid(cutTitleRight.size()/2);
            }
            else
            {
                title = cutTitleRight;
                newFontSizeRect = fm.boundingRect(ui->TitleButton->rect(), boundingRectFlags, prefix+title);
                curW = newFontSizeRect.width();
                if(curW > ui->TitleButton->width()) // if cutFileNameRight is already too big by itself
                    cutTitleLeft = ""; // we'll cutFileNameRight in two at next iteration
                else // if cutFileNameRight can still fit more char
                {
                    cutTitleRight = cutTitleLeft.mid(cutTitleLeft.size()/2) + cutTitleRight;
                    cutTitleLeft = cutTitleLeft.left(cutTitleLeft.size()/2);
                }
            }
        }
    }
    ui->TitleButton->setText(prefix+title);


//    if (vocabFileInfo.isDir() && !fakeUpDir)
//        title = "[DIR] " + vocabFileInfo.fileName(); // because hidden dir completeBaseName is empty

    // TODO NOW set title size automatically
//    ui->TitleButton->setText(title);
}

void VocabFileEntryWidget::FakeClick(bool checked)
{
    ui->checkBox->setChecked(checked);
    on_checkBox_clicked(checked);
}
