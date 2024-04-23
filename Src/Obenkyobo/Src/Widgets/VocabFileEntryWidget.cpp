#include <QList>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDirIterator>
#include "Src/Widgets/VocabFileEntryWidget.h"
#include "ui_VocabFileEntryWidget.h"
#include "Src/GetMy.h"
#include "Src/mainwindow.h"
#include "Src/Pages/VocabularyDisplayPage.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/Pages/VocabExplorerPage.h"
#include "Src/Tools.h"

VocabFileEntryWidget::VocabFileEntryWidget(QWidget *parent /*= nullptr*/) :
    QWidget(parent), ui(new Ui::VocabFileEntryWidget), vocabFileInfo(), initialPaintDone(false), scrollBarDisplayed(false)
{
    ui->setupUi(this);
}

VocabFileEntryWidget::VocabFileEntryWidget(QFileInfo fi, QWidget *parent)
    : QWidget(parent), ui(new Ui::VocabFileEntryWidget), vocabFileInfo(fi), initialPaintDone(false), scrollBarDisplayed(false)
{
    ui->setupUi(this);
    QString filename = vocabFileInfo.fileName();
    if (filename.isEmpty())
        ui->TitleButton->setText(""); // how tho ? (except for VocabFileUpDirWidget)
    else
    {
        QString prefix { (vocabFileInfo.isDir() ) ? "[DIR] " : ""};

        if (filename[0] == '.') // hidden file/folder
            ui->TitleButton->setText(prefix+'.'+vocabFileInfo.completeSuffix());
        else
            ui->TitleButton->setText(prefix+vocabFileInfo.completeBaseName());
    }

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

    ui->checkBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}").arg(VOCAB_FILE_ENTRY_TITLE_HEIGHT) );
    setMaximumHeight(VOCAB_FILE_ENTRY_TITLE_HEIGHT);

    SetAndTrimCurDirLabel();
}

VocabFileEntryWidget::~VocabFileEntryWidget()
{
    delete ui;
}

void VocabFileEntryWidget::SetLearningScoreText(QString learningScoreText)
{
    ui->checkBox->setText(learningScoreText);
}

void VocabFileEntryWidget::OnScrollbarEnabled()
{
    std::cout << "LOG: VocabExplorerPage::OnScrollbarEnabled()" << std::endl;
    scrollBarDisplayed = true;
    SetAndTrimCurDirLabel();
    ForceTitleButtonSize();
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
    std::cout << "DEBUG: ON_CLIC : contentsRect().width():" << ui->TitleButton->rect().width() << std::endl;
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
    // ugly, don't care for now, will have to separate VocabFileEntryWidget into its own daughter class at some point
    if (dynamic_cast<VocabFileUpDirWidget*>(this) != nullptr)
        std::cout << "LOG: VocabFileUpDirWidget::resizeEvent() BEGIN" << std::endl;
    else
        std::cout << "LOG: VocabExplorerPage::resizeEvent() BEGIN" << std::endl;

    QWidget::resizeEvent(event);

    if (!initialPaintDone)
        ForceTitleButtonSize();

    initialPaintDone = true;
}

void VocabFileEntryWidget::ForceTitleButtonSize()
{
    // Hack to prevent the TitleButton from expanding the whole VocabularyCfgListContentVLayout, fuck qt
    int correctHeight { VOCAB_FILE_ENTRY_TITLE_HEIGHT };
    int correctedWidth { static_cast<int>((GetMy::Instance().Descriptor().width - (scrollBarDisplayed ? GetMy::Instance().GetScrollBarSize() : 0))
                                        * VOCAB_FILE_ENTRY_TITLE_WIDTH_PCT) };

    ui->TitleButton->setSizePolicy({QSizePolicy::Fixed, QSizePolicy::Fixed});
    ui->TitleButton->setFixedSize(correctedWidth, correctHeight);
}

void VocabFileEntryWidget::SetAndTrimCurDirLabel()
{
    QString curLabelText { ui->TitleButton->text() };
    QFontMetricsF fm { ui->TitleButton->font() };
    int boundingRectFlags { 0 };
    QRectF newLabelRect { fm.boundingRect(
        QRect(0, 0, GetMy::Instance().Descriptor().width * VOCAB_FILE_ENTRY_TITLE_WIDTH_PCT
              , ui->TitleButton->rect().height()), boundingRectFlags, curLabelText) };
    qreal curFmRectWidth { newLabelRect.width() };
    int correctedWidth { static_cast<int>((GetMy::Instance().Descriptor().width - (scrollBarDisplayed ? GetMy::Instance().GetScrollBarSize() : 0))
                                          * VOCAB_FILE_ENTRY_TITLE_WIDTH_PCT) };
    if (curFmRectWidth > correctedWidth)
    {
        QString cutFileNameLeft = curLabelText.left(static_cast<int>(curLabelText.size()/2));
        QString cutFileNameRight = curLabelText.mid(static_cast<int>(curLabelText.size()/2));

        int railguard {0};
        while (railguard++ < SET_AND_TRIM_LOOPING_RAILGUARD && cutFileNameRight.size() != 1)
        {
            if (cutFileNameRight.size() == 0) // no right side => split leftinto two and loopback
            {
                cutFileNameRight = cutFileNameLeft.mid(static_cast<int>(cutFileNameLeft.size()/2));
                cutFileNameLeft = cutFileNameLeft.left(static_cast<int>(cutFileNameLeft.size()/2));
            }
            else
            {
                curLabelText = cutFileNameLeft + "[...]";
                newLabelRect = fm.boundingRect( QRect(0, 0, correctedWidth, ui->TitleButton->rect().height()), boundingRectFlags, curLabelText);
                curFmRectWidth = newLabelRect.width();
                if(curFmRectWidth > correctedWidth) // if cutFileNameLeft is already too big by itself
                    cutFileNameRight = ""; // we'll cutFileNameLeft in two at next iteration
                else // if cutFileNameLeft can still fit more char
                {
                    cutFileNameLeft = cutFileNameLeft + cutFileNameRight.left(static_cast<int>(cutFileNameRight.size()/2));
                    cutFileNameRight = cutFileNameRight.mid(static_cast<int>(cutFileNameRight.size()/2));
                }
            }
        }
        if (railguard >= SET_AND_TRIM_LOOPING_RAILGUARD)
            std::cerr << "VocabFileEntryWidget::SetAndTrimCurDirLabel IS STUCK LOOPING" << std::endl;
    }
    ui->TitleButton->setText(curLabelText);
}

void VocabFileEntryWidget::FakeClick(bool checked)
{
    ui->checkBox->setChecked(checked);
    on_checkBox_clicked(checked);
}

/*********************************************************************************************************************/

VocabFileUpDirWidget::VocabFileUpDirWidget(QFileInfo fileInfo, QWidget *parent) :
    VocabFileEntryWidget(fileInfo, parent)
{
    ui->TitleButton->setText("[UP_DIR] ..");
    ui->checkBox->setDisabled(true);
    ui->checkBox->setText("-");
}

VocabFileUpDirWidget::~VocabFileUpDirWidget()
{
}

void VocabFileUpDirWidget::OnScrollbarEnabled()
{
    std::cout << "LOG: VocabExplorerPage::OnScrollbarEnabled()" << std::endl;
    scrollBarDisplayed = true;
    ForceTitleButtonSize();
}

void VocabFileUpDirWidget::on_TitleButton_clicked()
{
    GetMy::Instance().VocabExplorerPageInst()->Populate(vocabFileInfo.filePath());
}
