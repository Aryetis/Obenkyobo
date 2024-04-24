#include <QScrollBar>
#include "Src/Pages/VocabExplorerPage.h"
#include "ui_VocabExplorerPage.h"
#include "Src/GetMy.h"
#include "Src/mainwindow.h"
#include "Src/VocabularyParser.h"
#include "Src/Widgets/VocabBaseEntryWidget.h"
#include "Src/Tools.h"
#include "Src/Pages/AppSettingsPage.h"

VocabExplorerPage::VocabExplorerPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabExplorerPage), initialPaintDone(false), selectAllStatus(false), homeLongPressTimer()
{
    ui->setupUi(this);

    // ************* currentDir stuff *************
    currentVocabDirString = GetMy::Instance().SettingSerializerInst()->value("vocab/currentDirectory", QString(QCoreApplication::applicationDirPath() + "/vocab/")).toString();
    currentDir = QDir(currentVocabDirString);

    ui->VocabularyCfgListContentVLayout->addStretch(); // TODO : for some reasons storing and inserting/removing SpacerItem at each Populate does not work because reasons... It's good enough for now

    homeLongPressTimer.setSingleShot(true);
    connect(ui->homeButton, &QPushButton::pressed, [&]{ homeLongPressTimer.start(VOCAB_EXPLORER_HOME_SET_PATH_TIMER_MS); });
    connect(ui->homeButton, &QPushButton::released, this, &VocabExplorerPage::HomeButtonLongPressReleased);
    connect(&homeLongPressTimer, &QTimer::timeout, this, &VocabExplorerPage::HomeButtonLongPressAction);

    GetMy::Instance().SetVocabExplorerPageInst(this);
}

void VocabExplorerPage::Populate(QDir dir)
{
    currentDir = dir;
    currentVocabDirString = dir.path();
    GetMy::Instance().SettingSerializerInst()->setValue("vocab/currentDirectory", currentVocabDirString);
    Populate();
}

void VocabExplorerPage::OnSleep() const
{
    if (GetMy::Instance().ToolsInst().GetDeviceState() != DeviceState::fakeSleeping)
        return;

    std::cout << "LOG: VocabExplorerPage::OnSleep()" << std::endl;
    disconnect(&homeLongPressTimer, nullptr, nullptr, nullptr);
}

void VocabExplorerPage::OnWakeUp() const
{
    if (GetMy::Instance().ToolsInst().GetDeviceState() != DeviceState::busy)
        return;

    std::cout << "LOG: VocabExplorerPage::OnWakeUp()" << std::endl;
    connect(&homeLongPressTimer, &QTimer::timeout, this, &VocabExplorerPage::HomeButtonLongPressAction);
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

    // ************* Set and trim labels  *************
    if (initialPaintDone)
    {
        SetAndTrimCurDirLabel();
    }

    // ************* Clearing *************
    qDeleteAll(vocabWidgets);
    vocabWidgets.clear();

    // ************* dirs *************
    QDir upDir = currentDir;
    if (upDir.cdUp())
    {
        VocabUpDirWidget* foo = new VocabUpDirWidget(QFileInfo(upDir, upDir.path()));
        vocabWidgets.push_back(foo);
        ui->VocabularyCfgListContentVLayout->insertWidget(0, foo);
    }

    foreach(const QFileInfo& dirInfo, currentDir.entryInfoList(QStringList() << "*", QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks))
    {
        VocabFileEntryWidget* bar = new VocabFileEntryWidget(dirInfo);
        vocabWidgets.push_back(bar);

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
        vocabWidgets.push_back(bar);

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

void VocabExplorerPage::SetAndTrimCurDirLabel()
{
    QString curDirLabelText {"Current Dir : "+currentVocabDirString};

    QFontMetricsF fm{ui->curDirLabel->font()};
    int boundingRectFlags = ui->curDirLabel != nullptr ? (ui->curDirLabel->wordWrap() ? Qt::TextWordWrap : 0) | ui->curDirLabel->alignment() : 0;
    QRectF newLabelRect {fm.boundingRect(ui->curDirLabel->rect(), boundingRectFlags, curDirLabelText)};
    qreal curW {newLabelRect.width()};
    bool finalCut = false;
    if (curW > ui->curDirLabel->width()) // if path is too big to fit on screen
    {
        while (curW > ui->curDirLabel->width() && !finalCut) // remove directory from path one by one unti it fits (or had to cut last dir)
        {
            int secondSlashPos = currentVocabDirString.indexOf('/', 1);
            if (secondSlashPos == -1) // if even the last directory by itself is still too big => cut it ...
            {
                QString cutFileNameLeft = currentVocabDirString.left(static_cast<int>(currentVocabDirString.size()/2));
                QString cutFileNameRight = currentVocabDirString.mid(static_cast<int>(currentVocabDirString.size()/2));

                int railguard {0};
                while (railguard++ < SET_AND_TRIM_LOOPING_RAILGUARD && cutFileNameLeft.size() != 1)
                {
                    if (cutFileNameLeft.size() == 0) // no left side => split right into two and loopback
                    {
                        cutFileNameLeft = cutFileNameRight.left(static_cast<int>(cutFileNameRight.size()/2));
                        cutFileNameRight = cutFileNameRight.mid(static_cast<int>(cutFileNameRight.size()/2));
                    }
                    else
                    {
                        curDirLabelText = "Current Dir : [...]"+cutFileNameRight;
                        newLabelRect = fm.boundingRect(ui->curDirLabel->rect(), boundingRectFlags, curDirLabelText);
                        curW = newLabelRect.width();
                        if(curW > ui->curDirLabel->width()) // if cutFileNameRight is already too big by itself
                            cutFileNameLeft = ""; // we'll cutFileNameRight in two at next iteration
                        else // if cutFileNameRight can still fit more char
                        {
                            cutFileNameRight = cutFileNameLeft.mid(static_cast<int>(cutFileNameLeft.size()/2)) + cutFileNameRight;
                            cutFileNameLeft = cutFileNameLeft.left(static_cast<int>(cutFileNameLeft.size()/2));
                        }
                    }
                }
                if (railguard >= SET_AND_TRIM_LOOPING_RAILGUARD)
                    std::cerr << "VocabFileEntryWidget::SetAndTrimCurDirLabel IS STUCK LOOPING" << std::endl;
                finalCut = true;
            }
            else // remove directory from path one by one (starting left)
            {
                currentVocabDirString = currentVocabDirString.right(currentVocabDirString.size()-secondSlashPos);

                curDirLabelText = "Current Dir : [...]"+currentVocabDirString;
                newLabelRect = fm.boundingRect(ui->curDirLabel->rect(), boundingRectFlags, curDirLabelText);
                curW = newLabelRect.width();
            }
        }
    }
    ui->curDirLabel->setText(curDirLabelText);
}

void VocabExplorerPage::HomeButtonLongPressReleased()
{
    if(homeLongPressTimer.isActive())
        homeLongPressTimer.stop();
}

void VocabExplorerPage::HomeButtonLongPressAction()
{
    std::cout << "LOG: VocabExplorerPage::HomeButtonLongPressAction" << std::endl;

    GetMy::Instance().AppSettingsPageInst().SetVocabExplorerHomePath(currentDir.path());
    GetMy::Instance().ToolsInst().DisplayPopup("Setting home path to :\n"+currentDir.path());
}

VocabExplorerPage::~VocabExplorerPage()
{
    for(VocabBaseEntryWidget* vw : vocabWidgets)
        delete vw;

    delete ui;
}

bool VocabExplorerPage::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->VocabularyCfgList->verticalScrollBar() && (event->type() == QEvent::Type::Show || event->type() == QEvent::Type::Hide))
    {
        GetMy::Instance().MainWindowInst().AggressiveClearScreen();
        for(VocabBaseEntryWidget* vw : vocabWidgets)
            vw->OnScrollbarToggled();
    }

    return false;
}

void VocabExplorerPage::resizeEvent(QResizeEvent *event)
{
    std::cout << "LOG: VocabExplorerPage::resizeEvent() BEGIN" << std::endl;
    QWidget::resizeEvent(event);

    initialPaintDone = true;

    SetAndTrimCurDirLabel();
}

void VocabExplorerPage::InitializeVocabularyLearnEditSet()
{
    Populate();
}

bool VocabExplorerPage::IsScrollBarDisplayed() const
{
    return ui->VocabularyCfgList->verticalScrollBar()->isVisible();
}

void VocabExplorerPage::on_SelectAllButton_clicked()
{
    selectAllStatus = !selectAllStatus;

    for(VocabBaseEntryWidget* vw : vocabWidgets)
        vw->FakeClick(selectAllStatus);
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
    currentVocabDirString = GetMy::Instance().AppSettingsPageInst().GetVocabExplorerHomePath();
    currentDir = QDir(currentVocabDirString);
    GetMy::Instance().SettingSerializerInst()->setValue("vocab/currentDirectory", currentVocabDirString);
    Populate();
}
