#include "vocabularylearneditset.h"
#include "ui_vocabularylearneditset.h"

#include <QDir>

VocabularyLearnEditSet::VocabularyLearnEditSet(QWidget *parent) :
    QWidget(parent), ui(new Ui::VocabularyLearnEditSet)
{
    ui->setupUi(this);

    QDir vocabDir(QString(QCoreApplication::applicationDirPath() + "/vocab/"));
    foreach(const QFileInfo& fileInfo, vocabDir.entryInfoList(QStringList() << "*.cfg", QDir::Files))
    {
        VocabularyCfgListEntry* foo = new VocabularyCfgListEntry(fileInfo);
        vocabCfgs.push_back(foo);

        ui->VocabularyCfgListContentVLayout->addWidget(foo);
    }
}

VocabularyLearnEditSet::~VocabularyLearnEditSet()
{
    for(VocabularyCfgListEntry* qs : vocabCfgs)
        delete qs;

    delete ui;
}
