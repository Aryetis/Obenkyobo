// #include "Src/Pages/NoteExplorerPage.h"
// #include "Src/GetMy.h"
// #include "Src/DefinesLand.h"
// #include "Src/Tools.h"
// #include "Src/Pages/AppSettingsPage.h"
// #include "Src/mainwindow.h"
// #include "Src/Widgets/FileEntryWidget.h"
// #include <QScrollBar>
// #include <ui_NoteExplorerPage.h>

// NoteExplorerPage::NoteExplorerPage(QWidget *parent) :
//     QWidget(parent), ui(new Ui::NoteExplorerPage), initialPaintDone(false)
// {
//     ui->setupUi(this);

//     // ************* currentDir stuff *************
//     QString currentNoteDirString = GetMy::Instance().SettingSerializerInst()->value("notes/currentDirectory", QString(QCoreApplication::applicationDirPath() + "/notes/")).toString();
//     currentDir = QDir(currentNoteDirString);

//     ui->NoteLIstContentVLayout->addStretch(); // TODO : for some reasons storing and inserting/removing SpacerItem at each Populate does not work because reasons... It's good enough for now

//     homeLongPressTimer.setSingleShot(true);
//     connect(ui->HomeButton, &QPushButton::pressed, [&]{ homeLongPressTimer.start(HOME_SET_PATH_TIMER_MS); });
//     connect(ui->HomeButton, &QPushButton::released, this, &NoteExplorerPage::HomeButtonLongPressReleased);
//     connect(&homeLongPressTimer, &QTimer::timeout, this, &NoteExplorerPage::HomeButtonLongPressAction);

//     GetMy::Instance().SetNoteExplorerPageInst(this);
// }

// void NoteExplorerPage::InitializeNoteExplorerPage()
// {
//     Populate();
// }

// void NoteExplorerPage::Populate(QDir dir)
// {
//     currentDir = dir;
//     GetMy::Instance().SettingSerializerInst()->setValue("notes/currentDirectory", dir.path());
//     Populate();
// }

// void NoteExplorerPage::Populate()
// {
//     // ************* Sanity Check current Dir *************
//     if (!currentDir.exists())
//         currentDir = QDir(QCoreApplication::applicationDirPath());

//     // ************* Set and trim labels  *************
//     if (initialPaintDone)
//     {
//         SetAndTrimCurDirLabel();
//     }

//     // ************* Clearing *************
//     DeleteEntryWidgets();
//     entryWidgets.clear();

//     // ************* dirs *************
//     QDir upDir = currentDir;
//     if (upDir.cdUp())
//     {
//         NoteFileUpDirWidget* foo = new NoteFileUpDirWidget(QFileInfo(upDir, upDir.path()));
//         entryWidgets.push_back(foo);
//         ui->NoteLIstContentVLayout->insertWidget(0, foo);
//     }

//     foreach(const QFileInfo& dirInfo, currentDir.entryInfoList(QStringList() << "*", QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks))
//     {
//         NoteFileEntryWidget* bar = new NoteFileEntryWidget(dirInfo);
//         entryWidgets.push_back(bar);
//         ui->NoteLIstContentVLayout->insertWidget(ui->NoteLIstContentVLayout->count()-1, bar);
//     }

//     // ************* *.txt || *.md *************
//     foreach(const QFileInfo& fileInfo, currentDir.entryInfoList(QStringList() << "*.txt" << "*.md", QDir::Files))
//     {
//         NoteFileEntryWidget* bar = new NoteFileEntryWidget(fileInfo);
//         entryWidgets.push_back(bar);
//         ui->NoteLIstContentVLayout->insertWidget(ui->NoteLIstContentVLayout->count()-1, (bar));
//     }

//     // ************* UI touch inputs stuff *************
//     connect( ui->NoteList->verticalScrollBar(), &QScrollBar::sliderReleased, this, &NoteExplorerPage::OnSliderReleased);
//     connect( ui->NoteList->verticalScrollBar(), &QScrollBar::valueChanged, this, &NoteExplorerPage::OnValueChanged);
//     ui->NoteList->verticalScrollBar()->installEventFilter(this);

//     ui->NoteList->setFocus(); // force focus on scrollbar so it handles physical buttons
// }

// void NoteExplorerPage::SetAndTrimCurDirLabel()
// {
//     QString currentNoteDirString = GetMy::Instance().SettingSerializerInst()->value("notes/currentDirectory", QString(QCoreApplication::applicationDirPath() + "/vocab/")).toString();
//     QString curDirLabelText {"Current Dir : "+currentNoteDirString};

//     QFontMetricsF fm{ui->CurDIrLabel->font()};
//     int boundingRectFlags = ui->CurDIrLabel != nullptr ? (ui->CurDIrLabel->wordWrap() ? Qt::TextWordWrap : 0) | ui->CurDIrLabel->alignment() : 0;
//     QRectF newLabelRect {fm.boundingRect(ui->CurDIrLabel->rect(), boundingRectFlags, curDirLabelText)};
//     qreal curW {newLabelRect.width()};
//     bool finalCut = false;
//     if (curW > ui->CurDIrLabel->width()) // if path is too big to fit on screen
//     {
//         while (curW > ui->CurDIrLabel->width() && !finalCut) // remove directory from path one by one unti it fits (or had to cut last dir)
//         {
//             int secondSlashPos = currentNoteDirString.indexOf('/', 1);
//             if (secondSlashPos == -1) // if even the last directory by itself is still too big => cut it ...
//             {
//                 QString cutFileNameLeft = currentNoteDirString.left(static_cast<int>(currentNoteDirString.size()/2));
//                 QString cutFileNameRight = currentNoteDirString.mid(static_cast<int>(currentNoteDirString.size()/2));

//                 int railguard {0};
//                 while (railguard++ < SET_AND_TRIM_LOOPING_RAILGUARD && cutFileNameLeft.size() != 1)
//                 {
//                     if (cutFileNameLeft.size() == 0) // no left side => split right into two and loopback
//                     {
//                         cutFileNameLeft = cutFileNameRight.left(static_cast<int>(cutFileNameRight.size()/2));
//                         cutFileNameRight = cutFileNameRight.mid(static_cast<int>(cutFileNameRight.size()/2));
//                     }
//                     else
//                     {
//                         curDirLabelText = "Current Dir : [...]"+cutFileNameRight;
//                         newLabelRect = fm.boundingRect(ui->CurDIrLabel->rect(), boundingRectFlags, curDirLabelText);
//                         curW = newLabelRect.width();
//                         if(curW > ui->CurDIrLabel->width()) // if cutFileNameRight is already too big by itself
//                             cutFileNameLeft = ""; // we'll cutFileNameRight in two at next iteration
//                         else // if cutFileNameRight can still fit more char
//                         {
//                             cutFileNameRight = cutFileNameLeft.mid(static_cast<int>(cutFileNameLeft.size()/2)) + cutFileNameRight;
//                             cutFileNameLeft = cutFileNameLeft.left(static_cast<int>(cutFileNameLeft.size()/2));
//                         }
//                     }
//                 }
//                 if (railguard >= SET_AND_TRIM_LOOPING_RAILGUARD)
//                     std::cerr << "NoteExplorerPage::SetAndTrimCurDirLabel IS STUCK LOOPING" << std::endl;
//                 finalCut = true;
//             }
//             else // remove directory from path one by one (starting left)
//             {
//                 currentNoteDirString = currentNoteDirString.right(currentNoteDirString.size()-secondSlashPos);

//                 curDirLabelText = "Current Dir : [...]"+currentNoteDirString;
//                 newLabelRect = fm.boundingRect(ui->CurDIrLabel->rect(), boundingRectFlags, curDirLabelText);
//                 curW = newLabelRect.width();
//             }
//         }
//     }
//     ui->CurDIrLabel->setText(curDirLabelText);
// }

// void NoteExplorerPage::HomeButtonLongPressReleased()
// {
//     if(homeLongPressTimer.isActive())
//         homeLongPressTimer.stop();
// }

// void NoteExplorerPage::HomeButtonLongPressAction()
// {
//     std::cout << "LOG: NoteExplorerPage::HomeButtonLongPressAction" << std::endl;

//     GetMy::Instance().AppSettingsPageInst().SetNoteExplorerHomePath(currentDir.path());
//     GetMy::Instance().ToolsInst().DisplayPopup("Setting home path to :\n"+currentDir.path());
// }

// void NoteExplorerPage::OnSliderReleased() const
// {
//     GetMy::Instance().MainWindowInst().AggressiveClearScreen();
// }

// void NoteExplorerPage::OnValueChanged(int /*value*/) const
// {
//     if (!ui->NoteList->verticalScrollBar()->isSliderDown())
//         GetMy::Instance().MainWindowInst().AggressiveClearScreen();
// }

// void NoteExplorerPage::DeleteEntryWidgets()
// {
//     for(BaseNoteFileEntryWidget* ew : entryWidgets)
//     {
//         if (NoteFileEntryWidget* nfew = dynamic_cast<NoteFileEntryWidget*>(ew))
//             delete nfew;
//         else if (NoteFileUpDirWidget* nfuw = dynamic_cast<NoteFileUpDirWidget*>(ew))
//             delete nfuw;
//         else
//             std::cerr << "NoteExplorerPage's vocabWidget is filled with junk, won't be cleared properly";
//     }

// }

// NoteExplorerPage::~NoteExplorerPage()
// {
//     DeleteEntryWidgets();
//     delete ui;
// }
