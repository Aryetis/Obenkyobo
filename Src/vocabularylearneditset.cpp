#include "vocabularylearneditset.h"
#include "ui_vocabularylearneditset.h"
#include "Src/GetMy.h"
#include "Src/vocabularydisplay.h"

#include <QDir>

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

    GetMy::Instance().SetVocabularyLearnEditSetWidget(this);
}

VocabularyLearnEditSet::~VocabularyLearnEditSet()
{
    for(VocabularyCfgListEntry* vc : vocabCfgs)
        delete vc;

    delete ui;
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
