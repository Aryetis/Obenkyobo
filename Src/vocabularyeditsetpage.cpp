#include "vocabularyeditsetpage.h"
#include "ui_vocabularyeditsetpage.h"

#include <QDir>
#include <QSettings>

#include "GetMy.h"

VocabularyEditSetPage::VocabularyEditSetPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VocabularyEditSetPage)
{
    ui->setupUi(this);

    QDir vocabDir(QString(QCoreApplication::applicationDirPath() + "/vocab"));
    QStringList vocabFiles = vocabDir.entryList(QStringList() << ".cfg", QDir::Files);
    for(QString& path : vocabFiles)
        vocabCfgs.push_back(QSettings(path, QSettings::IniFormat));
}

VocabularyEditSetPage::~VocabularyEditSetPage()
{
    delete ui;
}
