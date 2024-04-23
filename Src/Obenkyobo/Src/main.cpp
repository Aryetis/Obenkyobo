#include "Src/mainwindow.h"
#include <QApplication>
#include <QMessageLogger>
#include <QDateTime>
#include "Src/GetMy.h"
#include "Src/Tools.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GetMy::Instance().SetToolsInst(new Tools());
    GetMy::Instance().ToolsInst()->RegisterHandlers();
    GetMy::Instance().ToolsInst()->ParseKoboEreaderConf();

    std::cout << "Kobo model: (" << GetMy::Instance().Descriptor().modelName.toStdString() << ","
              << GetMy::Instance().Descriptor().modelNumber << "), firmware: " << GetMy::Instance().ToolsInst()->GetFirmwareStr() << std::endl;
    std::cout <<"Obenkyobo build: "<<__DATE__<<"@"<<__TIME__<<", cpp: "<<__cplusplus<<", Qt: "<<QT_VERSION<< std::endl;
    std::cout <<"Started at: " << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss").toStdString() << std::endl;

    // Needed to save .cfg next to application...
    QSettings serializer = QSettings(QString(QCoreApplication::applicationDirPath() + "/config.cfg"), QSettings::IniFormat);
    GetMy::Instance().SetSettingSerialierInst(&serializer);

    a.setStyleSheet
    (
// TODO replace scrollbar ugly icons with QStyle::SP_ArrowUp ?
        QString(
            /******************************** Scrollbar stuff ********************************/
            "QScrollBar                                                                                                 "
            "{ border: 2px solid grey; background: white; width: %1px; margin: %1px 0 %1px 0; }                         "
            "QScrollBar::handle                                                                                         "
            "{ background: rgb(60,60,60); min-height: 20px; }                                                           "
            "QScrollBar::add-line                                                                                       "
            "{ border: none; background: white; height: %1px; subcontrol-position: bottom; subcontrol-origin: margin; } "
            "QScrollBar::sub-line                                                                                       "
            "{ border: none; background: white; height: %1px; subcontrol-position: top; subcontrol-origin: margin; }    "
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
            /******************************** Checkbox stuff ********************************/
            "QCheckBox::indicator                                                                                       "
            "{                                                                                                          "
            "   border : 3px inset gray;                                                                                "
            "}                                                                                                          "
            "QCheckBox::indicator:indeterminate                                                                         "
            "{                                                                                                          "
                "image : url(:/pictures/Logos/CheckboxUndetermined.svg);                                                "
            "}                                                                                                          "
            "QCheckBox::indicator:checked                                                                               "
            "{                                                                                                          "
                "image : url(:/pictures/Logos/CheckboxChecked.svg);                                                     "
            "}                                                                                                          "
        ).arg( GetMy::Instance().GetScrollBarSize() )
    );

    MainWindow w;
    w.setFixedSize(GetMy::Instance().Descriptor().width, GetMy::Instance().Descriptor().height);
    w.show();
    bool ret = a.exec();
    return ret;
}
