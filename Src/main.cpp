#include "Src/mainwindow.h"
#include <QApplication>
#include <QMessageLogger>
#include <QDateTime>
#include "Src/GetMy.h"
#include "Src/tools.h"

int main(int argc, char *argv[])
{
    Tools::GetInstance().RegisterHandlers();

    std::cout <<"Obenkyobo build: "<<__DATE__<<"@"<<__TIME__<<", cpp: "<<__cplusplus<<", Qt: "<<QT_VERSION<< std::endl;
    std::cout <<"Started at: " << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss").toStdString() << std::endl;
    Tools::GetInstance().ParseKoboEreaderConf();

    QApplication a(argc, argv);
    std::cout << "Kobo model: (" << GetMy::Instance().Descriptor().modelName.toStdString() << ","
              << GetMy::Instance().Descriptor().modelNumber << "), firmware: " << Tools::GetInstance().GetFirmwareStr() << std::endl;

    // Needed to save .cfg next to application...
    QSettings serializer = QSettings(QString(QCoreApplication::applicationDirPath() + "/config.cfg"), QSettings::IniFormat);
    GetMy::Instance().SetSettingSerialier(&serializer);

    a.setStyleSheet
    (
        QString(
            /////////////// Scrollbar stuff
            "QScrollBar                                                                                                 "
            "{ border: 2px solid grey; background: white; width: %1px; margin: %1px 0 %1px 0; }                      "
            "QScrollBar::handle                                                                                         "
            "{ background: rgb(60,60,60); min-height: 20px; }                                                           "
            "QScrollBar::add-line                                                                                       "
            "{ border: none; background: white; height: %1px; subcontrol-position: bottom; subcontrol-origin: margin; }"
            "QScrollBar::sub-line                                                                                       "
            "{ border: none; background: white; height: %1px; subcontrol-position: top; subcontrol-origin: margin; }   "
            "QScrollBar::add-page, QScrollBar::sub-page                                                                 "
            "{ background: none; }                                                                                      "
            "                                                                                                           "
            "QScrollBar::up-arrow                                                                                       "
            "{                                                                                                          "
            "	image: url(:/pictures/Arrows/upArrow.png);                                                              "
            "	border: none; width: %1px; height: %1px; background: none;                                              "
            "}                                                                                                          "
            "QScrollBar::down-arrow                                                                                     "
            "{                                                                                                          "
            "	image: url(:/pictures/Arrows/downArrow.png);                                                            "
            "	border: none; width: %1px; height: %1px; background: none;                                              "
            "}                                                                                                          "
            "QScrollBar:left-arrow                                                                                      "
            "{                                                                                                          "
            "	image: url(:/pictures/Arrows/leftArrow.png);                                                            "
            "	border: none; width: %1px; height: %1px; background: none;                                              "
            "}                                                                                                          "
            "QScrollBar::right-arrow                                                                                    "
            "{                                                                                                          "
            "	image: url(:/pictures/Arrows/rightArrow.png);                                                           "
            "	border: none; width: %1px; height: %1px; background: none;                                              "
            "}                                                                                                          "
        ).arg( GetMy::Instance().Descriptor().width/10 )
    );

    MainWindow w;
    w.setFixedSize(GetMy::Instance().Descriptor().width, GetMy::Instance().Descriptor().height);
    w.show();
    return a.exec();
}
