#include "Src/Pages/NoteExplorerPage.h"
#include "Src/GetMy.h"
#include "Src/DefinesLand.h"
#include "Src/Tools.h"
#include "Src/Pages/AppSettingsPage.h"
#include <ui_NoteExplorerPage.h>

NoteExplorerPage::NoteExplorerPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::NoteExplorerPage)
{
    ui->setupUi(this);

    // ************* currentDir stuff *************
    QString currentVocabDirString = GetMy::Instance().SettingSerializerInst()->value("notes/currentDirectory", QString(QCoreApplication::applicationDirPath() + "/notes/")).toString();
    currentDir = QDir(currentVocabDirString);

    ui->NoteLIstContentVLayout->addStretch(); // TODO : for some reasons storing and inserting/removing SpacerItem at each Populate does not work because reasons... It's good enough for now

    homeLongPressTimer.setSingleShot(true);
    connect(ui->HomeButton, &QPushButton::pressed, [&]{ homeLongPressTimer.start(HOME_SET_PATH_TIMER_MS); });
    connect(ui->HomeButton, &QPushButton::released, this, &NoteExplorerPage::HomeButtonLongPressReleased);
    connect(&homeLongPressTimer, &QTimer::timeout, this, &NoteExplorerPage::HomeButtonLongPressAction);

    GetMy::Instance().SetNoteExplorerPageInst(this);
}

void NoteExplorerPage::InitializeNoteExplorerPage()
{

}

void NoteExplorerPage::HomeButtonLongPressReleased()
{
    if(homeLongPressTimer.isActive())
        homeLongPressTimer.stop();
}

void NoteExplorerPage::HomeButtonLongPressAction()
{
    std::cout << "LOG: NoteExplorerPage::HomeButtonLongPressAction" << std::endl;

    GetMy::Instance().AppSettingsPageInst().SetNoteExplorerHomePath(currentDir.path());
    GetMy::Instance().ToolsInst().DisplayPopup("Setting home path to :\n"+currentDir.path());
}

NoteExplorerPage::~NoteExplorerPage()
{
    delete ui;
}
