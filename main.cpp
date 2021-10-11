#include "mainwindow.h"
#include <QApplication>
#include "GetMy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Needed to save .cfg next to application...
    QSettings serializer = QSettings(QString(QCoreApplication::applicationDirPath() + "/config.cfg"), QSettings::IniFormat);
    GetMy::Instance().SetSettingSerialier(&serializer);

    MainWindow w;
    w.setFixedSize(GetMy::Instance().Descriptor().width, GetMy::Instance().Descriptor().height);
    w.show();
    return a.exec();
}
