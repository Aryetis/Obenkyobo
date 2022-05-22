#include "vocabularylearneditset.h"
#include "ui_vocabularylearneditset.h"
#include "Src/GetMy.h"
#include "Src/vocabularydisplay.h"
#include "Src/mainwindow.h"

#include <QScrollBar>

VocabularyLearnEditSet::VocabularyLearnEditSet(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyLearnEditSet), selectAllStatus(false)
{
    ui->setupUi(this);

    // ************* currentDir stuff *************
    currentVocabDirString = GetMy::Instance().SettingSerializer()->value("vocab/currentDirectory", QString(QCoreApplication::applicationDirPath() + "/vocab/")).toString();
    currentDir = QDir(currentVocabDirString);

    ui->VocabularyCfgListContentVLayout->addStretch(); // TODO : for some reasons storing and inserting/removing SpacerItem at each Populate does not work because reasons... It's good enough for now

    GetMy::Instance().SetVocabularyLearnEditSetWidget(this);
}

void VocabularyLearnEditSet::Populate(QDir dir)
{
    currentDir = dir;
    currentVocabDirString = dir.path();
    GetMy::Instance().SettingSerializer()->setValue("vocab/currentDirectory", currentVocabDirString);
    Populate();
}

void VocabularyLearnEditSet::Populate()
{
    // ************* Sanity Check current Dir *************
    if (!currentDir.exists())
    {
        currentVocabDirString = QString(QCoreApplication::applicationDirPath());
        currentDir = QDir(currentVocabDirString);
    }

    // ************* Clearing *************
    qDeleteAll(vocabCfgs);
    vocabCfgs.clear();

    // ************* curDirLabelText and top dir *************
    QString curDirLabelText = currentVocabDirString;
    if (curDirLabelText.size() > 15)
        curDirLabelText = "[...]" + curDirLabelText.right(15);
    ui->curDirLabel->setText("Current Dir : "+curDirLabelText);

    // ************* dirs *************
    QDir upDir = currentDir;
    if (upDir.cdUp())
    {
        VocabularyCfgListEntry* foo = new VocabularyCfgListEntry(QFileInfo(upDir, upDir.path()), true);
        vocabCfgs.push_back(foo);
        ui->VocabularyCfgListContentVLayout->insertWidget(0, foo);
    }

    foreach(const QFileInfo& fileInfo, currentDir.entryInfoList(QStringList() << "*", QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks ))
    {
        VocabularyCfgListEntry* bar = new VocabularyCfgListEntry(fileInfo);
        vocabCfgs.push_back(bar);

        ui->VocabularyCfgListContentVLayout->insertWidget(ui->VocabularyCfgListContentVLayout->count()-1, bar);
    }

    // ************* *.cfg *************
    foreach(const QFileInfo& fileInfo, currentDir.entryInfoList(QStringList() << "*.oben", QDir::Files))
    {
        VocabularyCfgListEntry* bar = new VocabularyCfgListEntry(fileInfo);
        vocabCfgs.push_back(bar);

        ui->VocabularyCfgListContentVLayout->insertWidget(ui->VocabularyCfgListContentVLayout->count()-1, (bar));
    }

    // ************* UI touch inputs stuff *************
    connect( ui->VocabularyCfgList->verticalScrollBar(), &QScrollBar::sliderReleased, this, &VocabularyLearnEditSet::OnSliderReleased);
    connect( ui->VocabularyCfgList->verticalScrollBar(), &QScrollBar::valueChanged, this, &VocabularyLearnEditSet::OnValueChanged);
    ui->VocabularyCfgList->verticalScrollBar()->installEventFilter(this);
}

VocabularyLearnEditSet::~VocabularyLearnEditSet()
{
    for(VocabularyCfgListEntry* vc : vocabCfgs)
        delete vc;

    delete ui;
}

bool VocabularyLearnEditSet::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->VocabularyCfgList->verticalScrollBar() && event->type() == QEvent::Type::Show)
        GetMy::Instance().MainWindowWidget().AggressiveClearScreen();

    return false;
}

void VocabularyLearnEditSet::InitializeVocabularyLearnEditSet()
{
    Populate();
    ui->VocabularyCfgList->setFocus(); // force focus on scrollbar so it handles physical buttons
}

void VocabularyLearnEditSet::on_SelectAllButton_clicked()
{
    selectAllStatus = !selectAllStatus;

    for (VocabularyCfgListEntry* vc : vocabCfgs)
        vc->FakeClick(selectAllStatus);
}

void VocabularyLearnEditSet::OnSliderReleased() const
{
    GetMy::Instance().MainWindowWidget().AggressiveClearScreen();
}

void VocabularyLearnEditSet::OnValueChanged(int /*value*/) const
{
    if (!ui->VocabularyCfgList->verticalScrollBar()->isSliderDown())
        GetMy::Instance().MainWindowWidget().AggressiveClearScreen();
}

void VocabularyLearnEditSet::on_pushButton_clicked()
{
    currentVocabDirString = QString(QCoreApplication::applicationDirPath());
    currentDir = QDir(currentVocabDirString);
    Populate();
}
