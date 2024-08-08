#include <QScrollBar>
#include "Src/Pages/ExplorerPage.h"
#include "ui_ExplorerPage.h"
#include "Src/GetMy.h"
#include "Src/mainwindow.h"
#include "Src/VocabularyParser.h"
#include "Src/Widgets/FileEntryWidget.h"
#include "Src/Tools.h"
#include "Src/Pages/AppSettingsPage.h"

BaseExplorerPage::BaseExplorerPage( QString curDirSerializedAdress_,
                                     QList<QString> extensions_,
                                     QWidget *parent /*= nullptr*/) :
    QWidget(parent), ui(new Ui::ExplorerPage), initialPaintDone(false), selectAllStatus(false), homeLongPressTimer(),
    curDirSerializedAdress(curDirSerializedAdress_), extensions(extensions_)
{
    ui->setupUi(this);

    // ************* currentDir stuff *************
    QString currentVocabDirString = GetMy::Instance().SettingSerializerInst()->value("vocab/currentDirectory", QString(QCoreApplication::applicationDirPath() + "/vocab/")).toString();
    currentDir = QDir(currentVocabDirString);

    ui->VocabularyCfgListContentVLayout->addStretch(); // TODO : for some reasons storing and inserting/removing SpacerItem at each Populate does not work because reasons... It's good enough for now

    homeLongPressTimer.setSingleShot(true);
    connect(ui->homeButton, &QPushButton::pressed, [&]{ homeLongPressTimer.start(HOME_SET_PATH_TIMER_MS); });
    connect(ui->homeButton, &QPushButton::released, this, &BaseExplorerPage::HomeButtonLongPressReleased);
    connect(&homeLongPressTimer, &QTimer::timeout, this, &BaseExplorerPage::HomeButtonLongPressAction);
}

void BaseExplorerPage::Populate(QDir dir)
{
    currentDir = dir;
    GetMy::Instance().SettingSerializerInst()->setValue("vocab/currentDirectory", dir.path());
    Populate();
}

void BaseExplorerPage::OnSleep() const
{
    if (GetMy::Instance().ToolsInst().GetDeviceState() != DeviceState::fakeSleeping)
        return;

    std::cout << "LOG: BaseExplorerPage::OnSleep()" << std::endl;
    disconnect(&homeLongPressTimer, nullptr, nullptr, nullptr);
}

void BaseExplorerPage::OnWakeUp() const
{
    if (GetMy::Instance().ToolsInst().GetDeviceState() != DeviceState::busy)
        return;

    std::cout << "LOG: BaseExplorerPage::OnWakeUp()" << std::endl;
    connect(&homeLongPressTimer, &QTimer::timeout, this, &BaseExplorerPage::HomeButtonLongPressAction);
}

void BaseExplorerPage::Populate()
{
    DisplayLSEnum displayLsSetting = GetMy::Instance().AppSettingsPageInst().GetDisplayLSSetting();

    // ************* Sanity Check current Dir *************
    if (!currentDir.exists())
        currentDir = QDir(QCoreApplication::applicationDirPath());

    // ************* Set and trim labels  *************
    if (initialPaintDone)
    {
        SetAndTrimCurDirLabel();
    }

    // ************* Clearing *************
    DeleteVocabWidgets();
    vocabWidgets.clear();

    // ************* dirs *************
    QDir upDir = currentDir;
    if (upDir.cdUp())
    {
        VocabFileUpDirWidget* foo = new VocabFileUpDirWidget(QFileInfo(upDir, upDir.path()));
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
    connect( ui->VocabularyCfgList->verticalScrollBar(), &QScrollBar::sliderReleased, this, &BaseExplorerPage::OnSliderReleased);
    connect( ui->VocabularyCfgList->verticalScrollBar(), &QScrollBar::valueChanged, this, &BaseExplorerPage::OnValueChanged);
    ui->VocabularyCfgList->verticalScrollBar()->installEventFilter(this);

    ui->VocabularyCfgList->setFocus(); // force focus on scrollbar so it handles physical buttons
}

void BaseExplorerPage::SetAndTrimCurDirLabel()
{
    QString currentVocabDirString = GetMy::Instance().SettingSerializerInst()->value("vocab/currentDirectory", QString(QCoreApplication::applicationDirPath() + "/vocab/")).toString();
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

void BaseExplorerPage::HomeButtonLongPressReleased()
{
    if(homeLongPressTimer.isActive())
        homeLongPressTimer.stop();
}

void BaseExplorerPage::HomeButtonLongPressAction()
{
    std::cout << "LOG: BaseExplorerPage::HomeButtonLongPressAction" << std::endl;

    GetMy::Instance().AppSettingsPageInst().SetVocabExplorerHomePath(currentDir.path());
    GetMy::Instance().ToolsInst().DisplayPopup("Setting home path to :\n"+currentDir.path());
}

void BaseExplorerPage::DeleteVocabWidgets()
{
    for(BaseVocabFileEntryWidget* vw : vocabWidgets)
    {
        if (VocabFileEntryWidget* vfew = dynamic_cast<VocabFileEntryWidget*>(vw))
            delete vfew;
        else if (VocabFileUpDirWidget* vfuw = dynamic_cast<VocabFileUpDirWidget*>(vw))
            delete vfuw;
        else
            std::cerr << "VocabExplorerPage's vocabWidget is filled with junk,  won't be cleared properly";
    }
}

BaseExplorerPage::~BaseExplorerPage()
{
    DeleteVocabWidgets();
    delete ui;
}

bool BaseExplorerPage::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->VocabularyCfgList->verticalScrollBar() && (event->type() == QEvent::Type::Show || event->type() == QEvent::Type::Hide))
    {
        GetMy::Instance().MainWindowInst().AggressiveClearScreen();
        for(BaseVocabFileEntryWidget* vw : vocabWidgets)
            vw->OnScrollbarToggled();
    }

    return false;
}

void BaseExplorerPage::resizeEvent(QResizeEvent *event)
{
    std::cout << "LOG: BaseExplorerPage::resizeEvent() BEGIN" << std::endl;
    QWidget::resizeEvent(event);

    initialPaintDone = true;

    SetAndTrimCurDirLabel();
}

void BaseExplorerPage::InitializePage()
{
    Populate();
}

bool BaseExplorerPage::IsScrollBarDisplayed() const
{
    return ui->VocabularyCfgList->verticalScrollBar()->isVisible();
}

void BaseExplorerPage::on_SelectAllButton_clicked()
{
    selectAllStatus = !selectAllStatus;

    for(BaseVocabFileEntryWidget* vw : vocabWidgets)
        vw->FakeClick(selectAllStatus);
}

void BaseExplorerPage::OnSliderReleased() const
{
    GetMy::Instance().MainWindowInst().AggressiveClearScreen();
}

void BaseExplorerPage::OnValueChanged(int /*value*/) const
{
    if (!ui->VocabularyCfgList->verticalScrollBar()->isSliderDown())
        GetMy::Instance().MainWindowInst().AggressiveClearScreen();
}

void BaseExplorerPage::on_homeButton_clicked()
{
    Populate(GetMy::Instance().AppSettingsPageInst().GetVocabExplorerHomePath());
}

/************************************* VocabExplorerPage ******************************************/

VocabExplorerPage::VocabExplorerPage(QWidget *parent)
    : BaseExplorerPage("vocab/currentDirectory", {{".oben"}}, parent)
{
    GetMy::Instance().SetVocabExplorerPageInst(this);
}

/************************************* NoteExplorerPage ******************************************/
NoteExplorerPage::NoteExplorerPage(QWidget *parent)
    : BaseExplorerPage("note/currentDirectory", {{".txt"}, {".md"}}, parent)
{
    GetMy::Instance().SetNoteExplorerPageInst(this);
}
