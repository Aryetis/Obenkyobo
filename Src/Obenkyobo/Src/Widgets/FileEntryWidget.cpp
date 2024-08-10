#include <QList>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDirIterator>
#include "Src/Widgets/FileEntryWidget.h"
#include "ui_VocabFileEntryWidget.h"
#include "Src/GetMy.h"
#include "Src/mainwindow.h"
#include "Src/Pages/VocabularyDisplayPage.h"
#include "Src/Pages/AppSettingsPage.h"
#include "Src/Pages/ExplorerPage.h"
#include "Src/Pages/NoteDisplayPage.h"
#include "Src/Tools.h"


/******************************************************************************************
 *                                  BaseFileEntryWidget                                   *
 ******************************************************************************************/

void BaseFileEntryWidget::ForceTitleButtonSize(QPushButton* target)
{
    // Hack to prevent the TitleButton from expanding the whole ListContentVLayout, fuck qt
    int correctHeight { FILE_ENTRY_TITLE_HEIGHT };
    int correctedWidth { static_cast<int>((GetMy::Instance().Descriptor().width - (IsScrollBarDisplayed() ? GetMy::Instance().GetScrollBarSize() : 0))
                                        * EntryWidth()) };

    target->setSizePolicy({QSizePolicy::Fixed, QSizePolicy::Fixed});
    target->setFixedSize(correctedWidth, correctHeight);
}

void BaseFileEntryWidget::BaseSetAndTrimCurDirLabel(QPushButton* target)
{
    QString curLabelText { target->text() };
    QFontMetricsF fm { target->font() };
    int boundingRectFlags { 0 };
    QRectF newLabelRect { fm.boundingRect(
        QRect(0, 0, GetMy::Instance().Descriptor().width * EntryWidth()
              , target->rect().height()), boundingRectFlags, curLabelText) };
    qreal curFmRectWidth { newLabelRect.width() };
    int correctedWidth { static_cast<int>((GetMy::Instance().Descriptor().width - (IsScrollBarDisplayed() ? GetMy::Instance().GetScrollBarSize() : 0))
                                        * EntryWidth()) };
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
                newLabelRect = fm.boundingRect( QRect(0, 0, correctedWidth, target->rect().height()), boundingRectFlags, curLabelText);
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
            std::cerr << "BaseFileEntryWidget::SetAndTrimCurDirLabel IS STUCK LOOPING" << std::endl;
    }
    target->setText(curLabelText);
}

/******************************************************************************************
 *                                BaseVocabFileEntryWidget                                *
 ******************************************************************************************/

BaseVocabFileEntryWidget::BaseVocabFileEntryWidget(QWidget *parent)
    : QWidget(parent), BaseFileEntryWidget(), ui(new Ui::BaseVocabFileEntryWidget)
{
    ui->setupUi(this);

    ui->checkBox->setStyleSheet( QString("QCheckBox::indicator { width: %1px; height: %1px;}").arg(FILE_ENTRY_TITLE_HEIGHT) );
    setMaximumHeight(FILE_ENTRY_TITLE_HEIGHT);
}

BaseVocabFileEntryWidget::~BaseVocabFileEntryWidget()
{
    delete ui;
}

void BaseVocabFileEntryWidget::ForceTitleButtonSize()
{
    BaseFileEntryWidget::ForceTitleButtonSize(ui->TitleButton);
}

float BaseVocabFileEntryWidget::EntryWidth()
{
    return VOCAB_FILE_ENTRY_TITLE_WIDTH_PCT;
}

bool BaseVocabFileEntryWidget::IsScrollBarDisplayed()
{
    return GetMy::Instance().VocabExplorerPageInst().IsScrollBarDisplayed();
}

void BaseVocabFileEntryWidget::SetLearningScoreText(QString /*learningScoreText*/)
{ }

void BaseVocabFileEntryWidget::on_checkBox_clicked(bool /*checked*/)
{ }

/******************************************************************************************
 *                                VocabFileEntryWidget                                    *
 ******************************************************************************************/

VocabFileEntryWidget::VocabFileEntryWidget(QFileInfo fileInfo_, QWidget *parent)
    : BaseVocabFileEntryWidget(parent)
{
    ForceTitleButtonSize();

    fileInfo = fileInfo_;

    QString filename = fileInfo.fileName();
    if (filename.isEmpty())
        ui->TitleButton->setText(""); // how tho ?
    else
    {
        QString prefix { (fileInfo.isDir()) ? "[DIR] " : ""};
        QString fileName { (fileInfo.suffix()=="oben") ? fileInfo.completeBaseName() : fileInfo.fileName()};
        ui->TitleButton->setText(prefix + fileName);
    }

    if  (fileInfo.isFile())
        ui->checkBox->setChecked(GetMy::Instance().GetEnabledVocabSheets().contains(fileInfo.absoluteFilePath()));
    else
    {
        ui->checkBox->setTristate(true);

        QDirIterator it(fileInfo.absoluteFilePath(), {"*.oben"}, QDir::Files, QDirIterator::Subdirectories);
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

    BaseFileEntryWidget::BaseSetAndTrimCurDirLabel(ui->TitleButton);
}

void VocabFileEntryWidget::OnScrollbarToggled()
{
    std::cout << "LOG: VocabFileEntryWidget::OnScrollbarToggled()" << std::endl;
    BaseFileEntryWidget::BaseSetAndTrimCurDirLabel(ui->TitleButton);
    ForceTitleButtonSize();
}

void VocabFileEntryWidget::SetLearningScoreText(QString learningScoreText)
{
    ui->checkBox->setText(learningScoreText);
}

void VocabFileEntryWidget::FakeClick(bool checked)
{
    ui->checkBox->setChecked(checked);
    on_checkBox_clicked(checked);
}

void VocabFileEntryWidget::on_TitleButton_clicked()
{
    if (fileInfo.isDir())
    {
        static_cast<BaseExplorerPage&>(GetMy::Instance().VocabExplorerPageInst()).Populate(fileInfo.filePath());
    }
    else
    {
        GetMy::Instance().VocabularyDisplayPageInst().InitializeGrid(this);
        GetMy::Instance().MainWindowInst().SwitchStackedWidgetIndex(8);

        if ( GetMy::Instance().AppSettingsPageInst().GetSettingsSerializer()->value("AppSettings/firstTimeVocabDisplayPage", true).toBool() )
        {
            GetMy::Instance().ToolsInst().DisplayPopup("You can click the top row buttons to hide/show each associated column.\n"
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
    if ( fileInfo.isFile() )
    {
        ui->checkBox->setChecked(checked);
        if (checked)
            GetMy::Instance().AddEnabledVocabSheet(fileInfo.absoluteFilePath());
        else
            GetMy::Instance().RemoveEnabledVocabSheet(fileInfo.absoluteFilePath());
    }
    else
    {
        ui->checkBox->setCheckState(checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
        QSet<QString> sheetSet;
        QDirIterator it(fileInfo.absoluteFilePath(), {"*.oben"}, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
            sheetSet << it.next();

        if (checked)
            GetMy::Instance().AddEnabledVocabSheets(sheetSet);
        else
            GetMy::Instance().RemoveEnabledVocabSheets(sheetSet);
    }
}

/******************************************************************************************
 *                                  VocabFileUpDirWidget                                  *
 ******************************************************************************************/

VocabFileUpDirWidget::VocabFileUpDirWidget(QFileInfo fileInfo_, QWidget *parent) :
    BaseVocabFileEntryWidget(parent)
{
    ForceTitleButtonSize();

    fileInfo = fileInfo_;

    ui->TitleButton->setText("[UP_DIR] ..");
    ui->checkBox->setDisabled(true);
    ui->checkBox->setText("-");
}

void VocabFileUpDirWidget::OnScrollbarToggled()
{
    std::cout << "LOG: VocabFileUpDirWidget::OnScrollbarToggled()" << std::endl;
    ForceTitleButtonSize();
}

void VocabFileUpDirWidget::on_TitleButton_clicked()
{
    static_cast<BaseExplorerPage&>(GetMy::Instance().VocabExplorerPageInst()).Populate(fileInfo.filePath());
}

/******************************************************************************************
 *                                 BaseNoteFileEntryWidget                                    *
 ******************************************************************************************/

BaseNoteFileEntryWidget::BaseNoteFileEntryWidget(QWidget *parent /*= nullptr*/) : QPushButton(parent)
{
    setMaximumHeight(FILE_ENTRY_TITLE_HEIGHT);

    connect(this, &QPushButton::released, this, &BaseNoteFileEntryWidget::OnClick);
};

BaseNoteFileEntryWidget::~BaseNoteFileEntryWidget()
{
    disconnect(this);
}

void BaseNoteFileEntryWidget::ForceTitleButtonSize()
{
    BaseFileEntryWidget::ForceTitleButtonSize(this);
}

float BaseNoteFileEntryWidget::EntryWidth()
{
    return 1.f;
};

bool BaseNoteFileEntryWidget::IsScrollBarDisplayed()
{
    return GetMy::Instance().NoteExplorerPageInst().IsScrollBarDisplayed();
}

/******************************************************************************************
 *                                 NoteFileEntryWidget                                    *
 ******************************************************************************************/

NoteFileEntryWidget::NoteFileEntryWidget(QFileInfo fileInfo_, QWidget *parent /*= nullptr*/)
    : BaseNoteFileEntryWidget(parent)
{
    ForceTitleButtonSize();

    fileInfo = fileInfo_;
    setText(fileInfo.fileName());

    QString filename = fileInfo.fileName();
    if (filename.isEmpty())
        setText(""); // how tho ?
    else
    {
        QString prefix { (fileInfo.isDir()) ? "[DIR] " : ""};
        setText(prefix + fileInfo.fileName()); // differentiate file with similar name but diff extensions
    }

    BaseFileEntryWidget::BaseSetAndTrimCurDirLabel(this);
}

void NoteFileEntryWidget::OnScrollbarToggled()
{
    std::cout << "LOG: NoteFileEntryWidget::OnScrollbarToggled()" << std::endl;
    BaseFileEntryWidget::BaseSetAndTrimCurDirLabel(this);
    ForceTitleButtonSize();
};

void NoteFileEntryWidget::OnClick()
{
    if (fileInfo.isDir())
    {
        static_cast<BaseExplorerPage&>(GetMy::Instance().NoteExplorerPageInst()).Populate(fileInfo.filePath());
    }
    else
    {
        GetMy::Instance().NoteDisplayPageInst().Populate(this);
        GetMy::Instance().MainWindowInst().SwitchStackedWidgetIndex(11);
    }
}

/******************************************************************************************
 *                                   NoteFileUpDirWidget                                  *
 ******************************************************************************************/

NoteFileUpDirWidget::NoteFileUpDirWidget(QFileInfo fileInfo_, QWidget *parent /*= nullptr*/)
    : BaseNoteFileEntryWidget(parent)
{
    ForceTitleButtonSize();

    fileInfo = fileInfo_;
    setText("[UP_DIR] ..");
}

void NoteFileUpDirWidget::OnScrollbarToggled()
{
    std::cout << "LOG: NoteFileUpDirWidget::OnScrollbarToggled()" << std::endl;
    ForceTitleButtonSize();
};

void NoteFileUpDirWidget::OnClick()
{
    static_cast<BaseExplorerPage&>(GetMy::Instance().NoteExplorerPageInst()).Populate(fileInfo.filePath());
}
