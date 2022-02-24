#-------------------------------------------------
# TODO : - update Readme.md
#-------------------------------------------------
# TODO later : - less janky popup messages height
#              - tweak MenuBar spacing and marging (probably not happening because of kobo quirk)
#              - handle sleep
#              - fix wifi
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Obenkyobo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

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
    Src/main.cpp \
    Src/mainwindow.cpp \
    Src/qsymbolpushbutton.cpp \
    Src/symbolsetting.cpp \
    Src/symbolsettingentry.cpp \
    Src/symbolsettingsection.cpp \
    Src/symbolstables.cpp \
    Src/vocabularycfglistentry.cpp \
    Src/vocabularydisplay.cpp \
    Src/vocabularylearneditset.cpp \
    Src/widgetwelcome.cpp \
    Src/qcmexercice.cpp \
    Src/keyboardexercice.cpp \
    Src/qcmentryguess.cpp \
    Src/appsettings.cpp \
    Src/fntsetting.cpp \
    Src/screensettings.cpp

RESOURCES += \
    Resources/HiraganaFonts.qrc \
    Resources/KanjiFonts.qrc \
    Resources/KatakanaFonts.qrc \
    Resources/RomanjiFonts.qrc \
    Resources/VersatileFonts.qrc \
    Resources/Pictures.qrc

HEADERS += \
    Src/GetMy.h \
    Src/mainwindow.h \
    Src/qsymbolpushbutton.h \
    Src/symbolsetting.h \
    Src/symbolsettingentry.h \
    Src/symbolsettingsection.h \
    Src/vocabularycfglistentry.h \
    Src/vocabularydisplay.h \
    Src/vocabularylearneditset.h \
    Src/widgetwelcome.h \
    Src/qcmexercice.h \
    Src/keyboardexercice.h \
    Src/qcmentryguess.h \
    Src/appsettings.h \
    Src/fntsetting.h \
    Src/screensettings.h \
    Src/symbolstables.h \
    Src/tools.h

OTHER_FILES += \
    OtherFiles/*

FORMS += \
    Src/mainwindow.ui \
    Src/symbolsetting.ui \
    Src/symbolsettingentry.ui \
    Src/symbolsettingsection.ui \
    Src/vocabularycfglistentry.ui \
    Src/vocabularydisplay.ui \
    Src/vocabularylearneditset.ui \
    Src/widgetwelcome.ui \
    Src/qcmexercice.ui \
    Src/keyboardexercice.ui \
    Src/qcmentryguess.ui \
    Src/appsettings.ui \
    Src/fntsetting.ui \
    Src/screensettings.ui

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
INSTALLS += target everythingButLibs thumbnail
#INSTALLS += target everythingButLibsAndSh thumbnail
#INSTALLS += everything thumbnail
#INSTALLS += target

DISTFILES +=
