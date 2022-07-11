#-------------------------------------------------
# TODO : - Refactor QcmExercice so it can handle AppSettings::enabledSheets
#        - Refactor VocabDisplayPage (fix UI shifting with LS, use a fontMetrics().boundingRect to determine the max lentgth of a title)
#        - Make VocabDisplayPage tweakable (Do not display / display only for current directory .oben / display for directories too)
#        - Vocab QCM needs a "do not resize if overflow" option in the FntSettings
#        - Fix errorTest.oben display empty line at top and bot
#        - Finish sleep handling properly
#-------------------------------------------------
# TODO later : - redo the Qmenu::right-arrow
#              - Investigate weird hard refresh (Edit page) glitch on Kobo Glo HD under unknown circumstances
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Obenkyobo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES +=\
    QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#qt5-kobo-platform-plugin stuff
INCLUDEPATH += $$PWD/Libs/qt5-kobo-platform-plugin/src

##########################################
CONFIG += c++17 \

QMAKE_CXXFLAGS += -g -rdynamic # for backtrace debuging

SOURCES += \
    Src/KanasTables.cpp \
    Src/Pages/AppSettingsPage.cpp \
    Src/Pages/FntSettingsPage.cpp \
    Src/Pages/KanaEditPage.cpp \
    Src/Pages/KeyboardExercicePage.cpp \
    Src/Pages/QcmExercicePage.cpp \
    Src/Pages/ScreenSettingsPage.cpp \
    Src/Pages/VocabExplorerPage.cpp \
    Src/Pages/VocabularyDisplayPage.cpp \
    Src/Pages/WelcomePage.cpp \
    Src/QcmDataEntryKana.cpp \
    Src/QcmDataEntryVocab.cpp \
    Src/VocabularyParser.cpp \
    Src/Widgets/KanaEditButtonWidget.cpp \
    Src/Widgets/KanaEditEntryWidget.cpp \
    Src/Widgets/KanaEditSectionWidget.cpp \
    Src/Widgets/PopupWidget.cpp \
    Src/Widgets/QcmEntryGuess.cpp \
    Src/Widgets/VocabFileEntryWidget.cpp \
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
    Src/Pages/VocabExplorerPage.h \
    Src/Pages/VocabularyDisplayPage.h \
    Src/Pages/WelcomePage.h \
    Src/QcmDataEntry.h \
    Src/QcmDataEntryKana.h \
    Src/QcmDataEntryVocab.h \
    Src/VocabularyParser.h \
    Src/Widgets/KanaEditButtonWidget.h \
    Src/Widgets/KanaEditEntryWidget.h \
    Src/Widgets/KanaEditSectionWidget.h \
    Src/Widgets/PopupWidget.h \
    Src/Widgets/QcmEntryGuess.h \
    Src/Widgets/VocabFileEntryWidget.h \
    Src/mainwindow.h \
    Src/tools.h

OTHER_FILES += \
    OtherFiles/*

FORMS += \
    Src/Pages/AppSettingsPage.ui \
    Src/Pages/FntSettingsPage.ui \
    Src/Pages/KanaEditPage.ui \
    Src/Pages/KeyboardExercicePage.ui \
    Src/Pages/QcmExercicePage.ui \
    Src/Pages/ScreenSettingsPage.ui \
    Src/Pages/VocabExplorerPage.ui \
    Src/Pages/VocabularyDisplayPage.ui \
    Src/Pages/WelcomePage.ui \
    Src/Widgets/KanaEditEntryWidget.ui \
    Src/Widgets/KanaEditSectionWidget.ui \
    Src/Widgets/PopupWidget.ui \
    Src/Widgets/QcmEntryGuess.ui \
    Src/Widgets/VocabFileEntryWidget.ui \
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
