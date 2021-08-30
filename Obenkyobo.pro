#-------------------------------------------------
#
# TODO : - guess hiragana / katakana from romanji mode (4 / 6 / 8 / 10 choices)
#        - guess romanji from hiragana / katakana mode
#        - custom selection mode
#        - 3 subcategories for hiragana / katakana tenten
#        - handakuten / dakuten mode / etc
#        - weighted random list
#        - tweak packager.sh so it creates a RTU .zip
#
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        widgetwelcome.cpp \
    qcmexercice.cpp \
    keyboardexercice.cpp \
    qcmentryguess.cpp \
    symbolsetting.cpp \
    appsettings.cpp \
    fntsetting.cpp \
    screensettings.cpp

RESOURCES += \
        Resources/fonts.qrc \
        Resources/pictures.qrc

HEADERS += \
        mainwindow.h \
        widgetwelcome.h \
    qcmexercice.h \
    keyboardexercice.h \
    qcmentryguess.h \
    symbolsetting.h \
    appsettings.h \
    fntsetting.h \
    screensettings.h \
    symbolstables.h \
    tools.h

OTHER_FILES += \
        OtherFiles/*

FORMS += \
        mainwindow.ui \
        widgetwelcome.ui \
    qcmexercice.ui \
    keyboardexercice.ui \
    qcmentryguess.ui \
    symbolsetting.ui \
    appsettings.ui \
    fntsetting.ui \
    screensettings.ui

# "everything" contains ... everything including qt libs (=>slow deployment)
everything.path = /mnt/onboard/.adds
everything.files = $$files($${OUT_PWD}/Output/.adds/**)

# "everythingButLibs" contains ... everything bug qt libs (=>fast deployment)
everythingButLibs.path = /mnt/onboard/.adds
everythingButLibs.files = $$files($${OUT_PWD}/Output/.adds/**)
everythingButLibs.files -= $$files($${OUT_PWD}/Output/.adds/qt-linux-*)

# just the thumbnail
thumbnail.path = /mnt/onboard/
thumbnail.files = $$files($${OUT_PWD}/Output/$${TARGET}.png)

# target contains only executable
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /mnt/onboard/.adds/$${TARGET}

#INSTALLS += target everythingButLibs thumbnail
INSTALLS += target

DISTFILES +=
