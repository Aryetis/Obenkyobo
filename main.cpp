#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
#ifdef QT_NO_DEBUG
    return a.exec();
#else
    bool ret = a.exec();
    system("reboot"); // otherwise the ereader will just hang up and requires reboot
    return ret;
#endif
}
