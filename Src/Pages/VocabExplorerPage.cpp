#include <QScrollBar>
#include "Src/Pages/VocabExplorerPage.h"
#include "ui_VocabExplorerPage.h"
#include "Src/GetMy.h"
#include "Src/mainwindow.h"
#include "Src/VocabularyParser.h"
#include "Src/Widgets/VocabFileEntryWidget.h"

#include "Src/Pages/AppSettingsPage.h"

VocabExplorerPage::VocabExplorerPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabExplorerPage), selectAllStatus(false)
{
    ui->setupUi(this);

    // ************* currentDir stuff *************
    currentVocabDirString = GetMy::Instance().SettingSerializerInst()->value("vocab/currentDirectory", QString(QCoreApplication::applicationDirPath() + "/vocab/")).toString();
    currentDir = QDir(currentVocabDirString);

    ui->VocabularyCfgListContentVLayout->addStretch(); // TODO : for some reasons storing and inserting/removing SpacerItem at each Populate does not work because reasons... It's good enough for now

    GetMy::Instance().SetVocabExplorerPageInst(this);
}

void VocabExplorerPage::Populate(QDir dir)
{
    currentDir = dir;
    currentVocabDirString = dir.path();
    GetMy::Instance().SettingSerializerInst()->setValue("vocab/currentDirectory", currentVocabDirString);
    Populate();
}

void VocabExplorerPage::Populate()
{
    DisplayLSEnum displayLsSetting = GetMy::Instance().AppSettingsPageInst().GetDisplayLSSetting();

    // ************* Sanity Check current Dir *************
    if (!currentDir.exists())
    {
        currentVocabDirString = QString(QCoreApplication::applicationDirPath());
        currentDir = QDir(currentVocabDirString);
    }

    // ************* Clearing *************
    qDeleteAll(vocabFileWidgets);
    vocabFileWidgets.clear();

    // ************* curDirLabelText and top dir *************
    QString curDirLabelText {"Current Dir : "+currentVocabDirString};

    QFontMetricsF fm{ui->curDirLabel->font()};
    int boundingRectFlags = ui->curDirLabel != nullptr ? (ui->curDirLabel->wordWrap() ? Qt::TextWordWrap : 0) | ui->curDirLabel->alignment() : 0;
    QRectF newFontSizeRect {fm.boundingRect(ui->curDirLabel->rect(), boundingRectFlags, curDirLabelText)};
    qreal curW {newFontSizeRect.width()};
    bool finalCut = false;
    if (curW > ui->curDirLabel->width()) // if path is too big to fit on screen
    {
        while (curW > ui->curDirLabel->width() && !finalCut) // remove directory from path one by one unti it fits (or had to cut last dir)
        {
            int secondSlashPos = currentVocabDirString.indexOf('/', 1);
            if (secondSlashPos == -1) // if even the last directory by itself is still too big => cut it ...
            {
                // TODO NOW MG cut in half until it fits (proper support for stupidly long folder name)
                QString cutFileNameLeft = currentVocabDirString.left(currentVocabDirString.size()/2);
                QString cutFileNameRight = currentVocabDirString.mid(currentVocabDirString.size()/2);

                while (cutFileNameLeft.size() != 1)
                {
                    if (cutFileNameLeft.size() == 0) // no left side => split right into two and loopback
                    {
                        cutFileNameLeft = cutFileNameRight.left(cutFileNameRight.size()/2);
                        cutFileNameRight = cutFileNameRight.mid(cutFileNameRight.size()/2);
                    }
                    else
                    {
                        curDirLabelText = "Current Dir : [...]"+cutFileNameRight;
                        newFontSizeRect = fm.boundingRect(ui->curDirLabel->rect(), boundingRectFlags, curDirLabelText);
                        curW = newFontSizeRect.width();
                        if(curW > ui->curDirLabel->width()) // if cutFileNameRight is already too big by itself
                            cutFileNameLeft = ""; // we'll cutFileNameRight in two at next iteration
                        else // if cutFileNameRight can still fit more char
                        {
                            cutFileNameRight = cutFileNameLeft.mid(cutFileNameLeft.size()/2) + cutFileNameRight;
                            cutFileNameLeft = cutFileNameLeft.left(cutFileNameLeft.size()/2);
                        }
                    }
                }
                finalCut = true;
            }
            else // remove directory from path one by one (starting left)
            {
                currentVocabDirString = currentVocabDirString.right(currentVocabDirString.size()-secondSlashPos);

                curDirLabelText = "Current Dir : [...]"+currentVocabDirString;
                newFontSizeRect = fm.boundingRect(ui->curDirLabel->rect(), boundingRectFlags, curDirLabelText);
                curW = newFontSizeRect.width();
            }
        }
    }
    ui->curDirLabel->setText(curDirLabelText);


    // ************* dirs *************
    QDir upDir = currentDir;
    if (upDir.cdUp())
    {
        VocabFileEntryWidget* foo = new VocabFileEntryWidget(QFileInfo(upDir, upDir.path()), true);
        vocabFileWidgets.push_back(foo);
        ui->VocabularyCfgListContentVLayout->insertWidget(0, foo);
    }

    foreach(const QFileInfo& dirInfo, currentDir.entryInfoList(QStringList() << "*", QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks))
    {
        VocabFileEntryWidget* bar = new VocabFileEntryWidget(dirInfo);
        vocabFileWidgets.push_back(bar);

        if (displayLsSetting == DisplayLSEnum::FilesAndDirs)
        {
            int avgLS = 0, fndOben = 0;
            QDirIterator it(dirInfo.absoluteFilePath(), QStringList() << "*.oben", QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext())
            {
                VocabDataFile vdf(it.next());
                avgLS += vdf.GetLearningScore();
                ++fndOben;
            }
            if (fndOben != 0)
                bar->SetLearningScoreText(QString::number(MAX_LEARNING_STATE_VALUE - avgLS/fndOben));
            else
                bar->SetLearningScoreText("-");
        }
        else
            bar->SetLearningScoreText("-");

        ui->VocabularyCfgListContentVLayout->insertWidget(ui->VocabularyCfgListContentVLayout->count()-1, bar);
    }

    // ************* *.oben *************
    foreach(const QFileInfo& fileInfo, currentDir.entryInfoList(QStringList() << "*.oben", QDir::Files))
    {
        VocabFileEntryWidget* bar = new VocabFileEntryWidget(fileInfo);
        vocabFileWidgets.push_back(bar);

        if (displayLsSetting != DisplayLSEnum::None)
        {
            VocabDataFile vdf(fileInfo.absoluteFilePath());
            bar->SetLearningScoreText(QString::number(MAX_LEARNING_STATE_VALUE - vdf.GetLearningScore()));
        }
        else
            bar->SetLearningScoreText("-");

        ui->VocabularyCfgListContentVLayout->insertWidget(ui->VocabularyCfgListContentVLayout->count()-1, (bar));
    }

    // ************* UI touch inputs stuff *************
    connect( ui->VocabularyCfgList->verticalScrollBar(), &QScrollBar::sliderReleased, this, &VocabExplorerPage::OnSliderReleased);
    connect( ui->VocabularyCfgList->verticalScrollBar(), &QScrollBar::valueChanged, this, &VocabExplorerPage::OnValueChanged);
    ui->VocabularyCfgList->verticalScrollBar()->installEventFilter(this);

    ui->VocabularyCfgList->setFocus(); // force focus on scrollbar so it handles physical buttons
}

VocabExplorerPage::~VocabExplorerPage()
{
    for(VocabFileEntryWidget* vc : vocabFileWidgets)
        delete vc;

    delete ui;
}

bool VocabExplorerPage::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->VocabularyCfgList->verticalScrollBar() && event->type() == QEvent::Type::Show)
        GetMy::Instance().MainWindowInst().AggressiveClearScreen();

    return false;
}

void VocabExplorerPage::InitializeVocabularyLearnEditSet()
{
    Populate();
}

void VocabExplorerPage::on_SelectAllButton_clicked()
{
    selectAllStatus = !selectAllStatus;

    for (int i = 1; i < static_cast<int>(vocabFileWidgets.size()); ++i) // skip [UP_DIR] entry // TODO handle [UP_DIR] correctly
        vocabFileWidgets[i]->FakeClick(selectAllStatus);
}

void VocabExplorerPage::OnSliderReleased() const
{
    GetMy::Instance().MainWindowInst().AggressiveClearScreen();
}

void VocabExplorerPage::OnValueChanged(int /*value*/) const
{
    if (!ui->VocabularyCfgList->verticalScrollBar()->isSliderDown())
        GetMy::Instance().MainWindowInst().AggressiveClearScreen();
}

void VocabExplorerPage::on_homeButton_clicked()
{
    currentVocabDirString = QString(QCoreApplication::applicationDirPath() + "/vocab/");
    currentDir = QDir(currentVocabDirString);
    GetMy::Instance().SettingSerializerInst()->setValue("vocab/currentDirectory", currentVocabDirString);
    Populate();
}
