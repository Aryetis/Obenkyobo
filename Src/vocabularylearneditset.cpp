#include "vocabularylearneditset.h"
#include "ui_vocabularylearneditset.h"
#include "Src/GetMy.h"
#include "Src/vocabularydisplay.h"
#include "Src/mainwindow.h"

#include <QDir>
#include <QScrollBar>


VocabularyLearnEditSet::VocabularyLearnEditSet(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyLearnEditSet), selectAllStatus(false)
{
    ui->setupUi(this);

    QDir vocabDir(QString(QCoreApplication::applicationDirPath() + "/vocab/"));
    foreach(const QFileInfo& fileInfo, vocabDir.entryInfoList(QStringList() << "*.cfg", QDir::Files))
    {
        VocabularyCfgListEntry* foo = new VocabularyCfgListEntry(fileInfo);
        vocabCfgs.push_back(foo);

        ui->VocabularyCfgListContentVLayout->addWidget(foo);
    }

    connect( ui->VocabularyCfgList->verticalScrollBar(), &QScrollBar::sliderReleased, this, &VocabularyLearnEditSet::OnSliderReleased);
    connect( ui->VocabularyCfgList->verticalScrollBar(), &QScrollBar::valueChanged, this, &VocabularyLearnEditSet::OnValueChanged);

    ui->VocabularyCfgList->verticalScrollBar()->installEventFilter(this);

    GetMy::Instance().SetVocabularyLearnEditSetWidget(this);
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
