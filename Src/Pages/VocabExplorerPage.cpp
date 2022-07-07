#include "Src/Pages/VocabExplorerPage.h"
#include "ui_VocabExplorerPage.h"
#include "Src/GetMy.h"
#include "Src/Pages/VocabularyDisplayPage.h"
#include "Src/mainwindow.h"

#include <QScrollBar>

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
    QString curDirLabelText = currentVocabDirString;
    if (curDirLabelText.size() > 16)
        curDirLabelText = "[...]" + curDirLabelText.right(16);
    ui->curDirLabel->setText("Current Dir : "+curDirLabelText);

    // ************* dirs *************
    QDir upDir = currentDir;
    if (upDir.cdUp())
    {
        VocabFileEntryWidget* foo = new VocabFileEntryWidget(QFileInfo(upDir, upDir.path()), true);
        vocabFileWidgets.push_back(foo);
        ui->VocabularyCfgListContentVLayout->insertWidget(0, foo);
    }

    foreach(const QFileInfo& fileInfo, currentDir.entryInfoList(QStringList() << "*", QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks ))
    {
        VocabFileEntryWidget* bar = new VocabFileEntryWidget(fileInfo);
        vocabFileWidgets.push_back(bar);

        ui->VocabularyCfgListContentVLayout->insertWidget(ui->VocabularyCfgListContentVLayout->count()-1, bar);
    }

    // ************* *.cfg *************
    foreach(const QFileInfo& fileInfo, currentDir.entryInfoList(QStringList() << "*.oben", QDir::Files))
    {
        VocabFileEntryWidget* bar = new VocabFileEntryWidget(fileInfo);
        vocabFileWidgets.push_back(bar);

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

    for (VocabFileEntryWidget* vc : vocabFileWidgets)
        vc->FakeClick(selectAllStatus);
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

void VocabExplorerPage::on_pushButton_clicked()
{
    currentVocabDirString = QString(QCoreApplication::applicationDirPath() + "/vocab/");
    currentDir = QDir(currentVocabDirString);
    Populate();
}
