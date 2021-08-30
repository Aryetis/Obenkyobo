#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

#include <QString>
#include <QDateTime>
#include <QFile>

#include <iostream>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   Q_UNUSED(context);

   QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString txt = QString("[%1] ").arg(dt);

   switch (type)
   {
      case QtDebugMsg:
         txt += QString("{Debug} \t\t %1").arg(msg);
         break;
      case QtWarningMsg:
         txt += QString("{Warning} \t %1").arg(msg);
         break;
      case QtCriticalMsg:
         txt += QString("{Critical} \t %1").arg(msg);
         break;
      case QtFatalMsg:
         txt += QString("{Fatal} \t\t %1").arg(msg);
         abort();
         break;
   }

   QFile outFile("/mnt/onboard/.adds/Obenkyobo/LogFile.log");
   outFile.open(QIODevice::WriteOnly | QIODevice::Append);

   QTextStream textStream(&outFile);
   textStream << txt << endl;
}

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(customMessageHandler);
    //TODO test that error handler before QApplication stuff ...

    std::cout << "Test" << std::endl;

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
