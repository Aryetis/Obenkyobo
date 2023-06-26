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

//    ui->TitleButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
//    ui->TitleButton->setFixedWidth((GetMy::Instance().Descriptor().width/100)*90);
//    ui->checkBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
//    ui->checkBox->setFixedWidth((GetMy::Instance().Descriptor().width/100)*10);
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
}

VocabFileEntryWidget::~VocabFileEntryWidget()
{
    delete ui;
}

void VocabFileEntryWidget::SetLearningScoreText(QString learningScoreText)
{
    ui->checkBox->setText(learningScoreText);
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
    std::cout << "DEBUG: ON_CLIC : contentsRect().width():" << ui->TitleButton->contentsRect().width() << std::endl;
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

    // another dirty hack because koboQT... for some reasons I can't use TitleButton.height to set checkbox's one
    ui->checkBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                                 .arg(GetMy::Instance().Descriptor().height/20) );
    setMaximumHeight(GetMy::Instance().Descriptor().height/20);

    Tools::DebugLog("DEBUG: ON_RESIZE, contentsRect().width() : "+QString::number(ui->TitleButton->contentsRect().width()));
    Tools::DebugLog("DEBUG: ON_RESIZE, (GetMy::Instance().Descriptor().width/100)*90 : "+
                    QString::number((GetMy::Instance().Descriptor().width/100)*90));
    // ********************* Trim and cut off button's text *********************
//    QString prefix { (vocabFileInfo.isDir() && !fakeUpDir) ? "[DIR] " : ""};

//    QFontMetricsF fm{ui->TitleButton->font()};
//    int boundingRectFlags = 0;
//    QRectF newTitleRect {fm.boundingRect(ui->TitleButton->contentsRect(), boundingRectFlags, prefix+title)};
//    qreal curW {newTitleRect.width()};
//float cntRectWidth = ui->TitleButton->contentsRect().width();
//    if (curW > ui->TitleButton->contentsRect().width()) // if path is too big to fit on screen
//    {
//        // TODO WHAT THE FUCK !!!! WHERE IS MY [...] at first draw ?
//        prefix = (vocabFileInfo.isDir()) ? "[DIR] [...]" : "[...]"; // fakeUpDir can't be too long.
//        QString cutTitleLeft = title.left(static_cast<int>(title.size()/2));
//        QString cutTitleRight = title.mid(static_cast<int>(title.size()/2));

//        while (cutTitleLeft.size() != 1)
//        {
//            if (cutTitleLeft.size() == 0) // no left side => split right into two and loopback
//            {
//                cutTitleLeft = cutTitleRight.left(static_cast<int>(cutTitleRight.size()/2));
//                cutTitleRight = cutTitleRight.mid(static_cast<int>(cutTitleRight.size()/2));
//            }
//            else
//            {
//                title = cutTitleRight;
//                newTitleRect = fm.boundingRect(ui->TitleButton->contentsRect(), boundingRectFlags, prefix+title);
//                curW = newTitleRect.width();
//                if(curW > ui->TitleButton->contentsRect().width()) // if cutFileNameRight is already too big by itself
//                    cutTitleLeft = ""; // we'll cutFileNameRight in two at next iteration
//                else // if cutFileNameRight can still fit more char
//                {
//                    cutTitleRight = cutTitleLeft.mid(static_cast<int>(cutTitleLeft.size()/2)) + cutTitleRight;
//                    cutTitleLeft = cutTitleLeft.left(static_cast<int>(cutTitleLeft.size()/2));
//                }
//            }
//        }
//newTitleRect = fm.boundingRect(ui->TitleButton->contentsRect(), boundingRectFlags, prefix+title);
//curW = newTitleRect.width();
//if(curW > ui->TitleButton->contentsRect().width()) // if cutFileNameRight is already too big by itself
//std::cout << "FUCK" << ui->TitleButton->contentsRect().width() << std::endl;
//    }
//    ui->TitleButton->setText(prefix+title);

float cntRectWidth = ui->TitleButton->contentsRect().width();
// ********************* Trim and cut off button's text *********************
QString prefix { (vocabFileInfo.isDir() && !fakeUpDir) ? "[DIR] " : ""};
QFontMetricsF fm{ui->TitleButton->font()};
int boundingRectFlags = 0;
QString wholeTitle {prefix+title};
QString wipTitle {wholeTitle};
auto DoesItFit = [&](auto testTitle)
{
    QRectF newTitleRect {fm.boundingRect(ui->TitleButton->contentsRect(), boundingRectFlags, testTitle)};
    qreal curW {newTitleRect.width()};
    Tools::DebugLog("DEBUG: ON_RESIZE, DoesItFit, curW : "+QString::number(curW));
    return curW <= (GetMy::Instance().Descriptor().width/100)*90;
//    return curW <= ui->TitleButton->contentsRect().width();
};

int cutPoint = static_cast<int>(wipTitle.size()/2);
int step = cutPoint;
bool doesItFit = DoesItFit(wipTitle);
if(!doesItFit)
{
    // TODO change prefix and wipTitle;
    bool didItFit = true;
    while(step > 1)
    {
        didItFit =  doesItFit;
        int foo = static_cast<int>(step/2);
        step = (foo > 0) ? foo : 1;
        if (doesItFit)
            cutPoint += step;
        else if (didItFit)
            cutPoint -= step;

        wipTitle = wholeTitle.mid(cutPoint);
        doesItFit = DoesItFit(wipTitle);
        Tools::DebugLog("DEBUG: ON_RESIZE, evolving wipTitle : "+wipTitle);
        Tools::DebugLog("DEBUG: ON_RESIZE, doesItFit : "+QString::number(doesItFit)+" ; didItFit : "+QString::number(didItFit));
    }
}
Tools::DebugLog("DEBUG: ON_RESIZE, final wipTitle : "+wipTitle);
ui->TitleButton->setText(wipTitle);
}

void VocabFileEntryWidget::FakeClick(bool checked)
{
    ui->checkBox->setChecked(checked);
    on_checkBox_clicked(checked);
}
