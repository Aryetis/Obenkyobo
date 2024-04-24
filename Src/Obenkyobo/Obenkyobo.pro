QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Obenkyobo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

#qt5-kobo-platform-plugin stuff
INCLUDEPATH += $$PWD/../Libs/qt5-kobo-platform-plugin/src

# packager.sh release [/]Obenkyobo/Src/Obenkyobo [/]Obenkyobo/build-ObenkyoboProject-KoboLibraH2o-Release/Src/Obenkyobo Obenkyobo
CONFIG(debug, debug|release) {
QMAKE_POST_LINK += $$PWD/OtherFiles/packager.sh debug $$PWD $$OUT_PWD $$TARGET
} else {
QMAKE_POST_LINK += $$PWD/OtherFiles/packager.sh release $$PWD $$OUT_PWD $$TARGET
}

##########################################
CONFIG += c++17 \

QMAKE_CXXFLAGS += -g -rdynamic # for backtrace debuging

SOURCES += \
    Src/GetMy.cpp \
    Src/KanasTables.cpp \
    Src/Pages/AppSettingsPage.cpp \
    Src/Pages/FntSettingsPage.cpp \
    Src/Pages/KanaEditPage.cpp \
    Src/Pages/KeyboardExercicePage.cpp \
    Src/Pages/QcmExercicePage.cpp \
    Src/Pages/ScreenSettingsPage.cpp \
    Src/Pages/StatisticsPage.cpp \
    Src/Pages/VocabExplorerPage.cpp \
    Src/Pages/VocabularyDisplayPage.cpp \
    Src/Pages/WelcomePage.cpp \
    Src/QcmDataEntryKana.cpp \
    Src/Tools.cpp \
    Src/VocabularyParser.cpp \
    Src/Widgets/KanaEditButtonWidget.cpp \
    Src/Widgets/KanaEditEntryWidget.cpp \
    Src/Widgets/KanaEditSectionWidget.cpp \
    Src/Widgets/PopupWidget.cpp \
    Src/Widgets/QcmEntryGuess.cpp \
    Src/Widgets/VocabBaseEntryWidget.cpp \
    Src/main.cpp \
    Src/mainwindow.cpp

RESOURCES += \
    Resources/HiraganaFonts.qrc \
    Resources/KanjiFonts.qrc \
    Resources/KatakanaFonts.qrc \
    Resources/RomanjiFonts.qrc \
    Resources/VersatileFonts.qrc \
    Resources/Pictures.qrc

HEADERS += \
    Src/DefinesLand.h \
    Src/GetMy.h \
    Src/KanasTables.h \
    Src/Pages/AppSettingsPage.h \
    Src/Pages/FntSettingsPage.h \
    Src/Pages/KanaEditPage.h \
    Src/Pages/KeyboardExercicePage.h \
    Src/Pages/QcmExercicePage.h \
    Src/Pages/ScreenSettingsPage.h \
    Src/Pages/StatisticsPage.h \
    Src/Pages/VocabExplorerPage.h \
    Src/Pages/VocabularyDisplayPage.h \
    Src/Pages/WelcomePage.h \
    Src/QcmDataEntry.h \
    Src/QcmDataEntryKana.h \
    Src/Tools.h \
    Src/VocabularyParser.h \
    Src/Widgets/KanaEditButtonWidget.h \
    Src/Widgets/KanaEditEntryWidget.h \
    Src/Widgets/KanaEditSectionWidget.h \
    Src/Widgets/PopupWidget.h \
    Src/Widgets/QcmEntryGuess.h \
    Src/Widgets/VocabBaseEntryWidget.h \
    Src/mainwindow.h

OTHER_FILES += \
    OtherFiles/*

FORMS += \
    Src/Pages/AppSettingsPage.ui \
    Src/Pages/FntSettingsPage.ui \
    Src/Pages/KanaEditPage.ui \
    Src/Pages/KeyboardExercicePage.ui \
    Src/Pages/QcmExercicePage.ui \
    Src/Pages/ScreenSettingsPage.ui \
    Src/Pages/StatisticsPage.ui \
    Src/Pages/VocabExplorerPage.ui \
    Src/Pages/VocabularyDisplayPage.ui \
    Src/Pages/WelcomePage.ui \
    Src/Widgets/KanaEditEntryWidget.ui \
    Src/Widgets/KanaEditSectionWidget.ui \
    Src/Widgets/PopupWidget.ui \
    Src/Widgets/QcmEntryGuess.ui \
    Src/Widgets/VocabBaseEntryWidget.ui \
    Src/mainwindow.ui

# "everything" contains ... everything including qt libs (=>slow deployment)
everything.path = /mnt/onboard/.adds
everything.files = $$files($${OUT_PWD}/Output/.adds/**)

# "everythingButLibs" contains ... everything bug qt libs (=>fast deployment)
everythingButLibs.path = /mnt/onboard/.adds
everythingButLibs.files = $$files($${OUT_PWD}/Output/.adds/**)
everythingButLibs.files -= $$files($${OUT_PWD}/Output/.adds/qt-linux-*)

# "everythingButLibsAndSh" contains ... everything bug qt libs and .sh launcher script (=>fast deployment)
everythingButLibsAndSh.path = /mnt/onboard/.adds
everythingButLibsAndSh.files = $$files($${OUT_PWD}/Output/.adds/**)
everythingButLibsAndSh.files -= $$files($${OUT_PWD}/Output/.adds/qt-linux-*)
everythingButLibsAndSh.files -= $$files($${OUT_PWD}/Output/.adds/Obenkyobo/Obenkyobo_launcher.sh)

# just the thumbnail
thumbnail.path = /mnt/onboard/
thumbnail.files = $$files($${OUT_PWD}/Output/$${TARGET}.png)

# target contains only executable
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /mnt/onboard/.adds/$${TARGET}

##################### install everything first then only deploy binary/target #####################
#INSTALLS += target everything thumbnail
#INSTALLS += target everythingButLibs thumbnail
#INSTALLS += target everythingButLibsAndSh thumbnail
#INSTALLS += everything thumbnail
INSTALLS += target

DISTFILES +=
