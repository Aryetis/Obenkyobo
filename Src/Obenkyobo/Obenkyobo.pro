QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Obenkyobo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/../Libs/qt5-kobo-platform-plugin/src \
               $$PWD/../Libs/KoboExtraFunk/src

DEPENDPATH += $$PWD/../Libs/KoboExtraFunk/src
LIBS += -L$$OUT_PWD/../Libs/KoboExtraFunk/ -lKoboExtraFunk

# packager.sh release [/]Obenkyobo/Src/Obenkyobo [/]Obenkyobo/build-ObenkyoboProject-KoboLibraH2o-Release/Src/Obenkyobo Obenkyobo
CONFIG(debug, debug|release) {
QMAKE_POST_LINK += $$PWD/OtherFiles/packager.sh debug $$PWD $$OUT_PWD $$TARGET
} else {
QMAKE_POST_LINK += $$PWD/OtherFiles/packager.sh release $$PWD $$OUT_PWD $$TARGET
}

##########################################
CONFIG += c++17 \

QMAKE_CXXFLAGS += -g -rdynamic # for backtrace debuging log

SOURCES += \
    Src/GetMy.cpp \
    Src/KanasTables.cpp \
    Src/Pages/AppSettingsPage.cpp \
    Src/Pages/ExplorerPage.cpp \
    Src/Pages/FntSettingsPage.cpp \
    Src/Pages/KanaEditPage.cpp \
    Src/Pages/KeyboardExercicePage.cpp \
    Src/Pages/NoteDisplayPage.cpp \
    Src/Pages/NoteExplorerPage.cpp \
    Src/Pages/QcmExercicePage.cpp \
    Src/Pages/ScreenSettingsPage.cpp \
    Src/Pages/StatisticsPage.cpp \
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
    Src/Widgets/FileEntryWidget.cpp \
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
    Src/Pages/ExplorerPage.h \
    Src/Pages/FntSettingsPage.h \
    Src/Pages/KanaEditPage.h \
    Src/Pages/KeyboardExercicePage.h \
    Src/Pages/NoteDisplayPage.h \
    Src/Pages/NoteExplorerPage.h \
    Src/Pages/QcmExercicePage.h \
    Src/Pages/ScreenSettingsPage.h \
    Src/Pages/StatisticsPage.h \
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
    Src/Widgets/FileEntryWidget.h \
    Src/mainwindow.h

OTHER_FILES += \
    OtherFiles/resourcesListing.txt \
    OtherFiles/packager.sh \
    OtherFiles/RemoteScripts/* \
    OtherFiles/vocab/*.oben

FORMS += \
    Src/Pages/AppSettingsPage.ui \
    Src/Pages/ExplorerPage.ui \
    Src/Pages/FntSettingsPage.ui \
    Src/Pages/KanaEditPage.ui \
    Src/Pages/KeyboardExercicePage.ui \
    Src/Pages/QcmExercicePage.ui \
    Src/Pages/ScreenSettingsPage.ui \
    Src/Pages/StatisticsPage.ui \
    Src/Pages/VocabularyDisplayPage.ui \
    Src/Pages/NoteExplorerPage.ui \
    Src/Pages/NoteDisplayPage.ui \
    Src/Pages/WelcomePage.ui \
    Src/Widgets/KanaEditEntryWidget.ui \
    Src/Widgets/KanaEditSectionWidget.ui \
    Src/Widgets/PopupWidget.ui \
    Src/Widgets/QcmEntryGuess.ui \
    Src/Widgets/VocabFileEntryWidget.ui \
    Src/mainwindow.ui

target.path = /mnt/onboard/.adds/$${TARGET}
INSTALLS += target

DISTFILES +=
