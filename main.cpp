#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
//    QFontDatabase db;
//    for(int i=0; i<db.families().size(); i++)
//    {
//      qDebug() << db.families().at(i);
//    }

    QApplication a(argc, argv);
//    QFontDatabase::addApplicationFont(":/fonts/HIRAGANA.ttf");
//    QFontDatabase::addApplicationFont(":/fonts/Hiragana_Scripts.ttf");
//    QFontDatabase::addApplicationFont(":/fonts/hiragana_ftb.ttf");

//    int id = QFontDatabase::addApplicationFont(":/fonts/HIRAGANA.ttf");


    MainWindow w;
    w.show();

    return a.exec();
}
