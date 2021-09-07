#include "mainwindow.h"
#include <QApplication>
#include "GetMy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings serializer = QSettings(QString(QCoreApplication::applicationDirPath() + "/config.cfg"), QSettings::IniFormat);
    GetMy::GetInstance().SetSettingSerialier(&serializer);

    MainWindow w;
    w.setFixedSize(GetMy::GetInstance().Descriptor().width, GetMy::GetInstance().Descriptor().height);
    w.show();
#ifdef QT_NO_DEBUG
    return a.exec();
#else
    bool ret = a.exec();
    system("reboot"); // otherwise the ereader will just hang up and requires reboot
    return ret;
#endif
}
