#-------------------------------------------------
#
# TODO : - handle sleep
#        - Wifi switch (for debug or background dl) KoboPlatformFunctions::disable/ enableWiFiConnection
#        - handle QTPA dependency properly, don't have a copy paste from some version and possible src of other versions, get bin from compiled src
#        - redirect stack trace to std::cerr
#        - link physical button to scrollbar without having to click scrollbar first
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

#qt5-kobo-platform-plugin stuff
INCLUDEPATH += $$PWD/libs/qt5-kobo-platform-plugin/src

##########################################
CONFIG += c++17 \

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qsymbolpushbutton.cpp \
    symbolsetting.cpp \
    symbolsettingentry.cpp \
    symbolsettingsection.cpp \
    symbolstables.cpp \
    widgetwelcome.cpp \
    qcmexercice.cpp \
    keyboardexercice.cpp \
    qcmentryguess.cpp \
    appsettings.cpp \
    fntsetting.cpp \
    screensettings.cpp

RESOURCES += \
    Resources/HiraganaFonts.qrc \
    Resources/KatakanaFonts.qrc \
    Resources/RomanjiFonts.qrc \
    Resources/VersatileFonts.qrc \
    Resources/Pictures.qrc

HEADERS += \
    GetMy.h \
    mainwindow.h \
    qsymbolpushbutton.h \
    symbolsetting.h \
    symbolsettingentry.h \
    symbolsettingsection.h \
    widgetwelcome.h \
    qcmexercice.h \
    keyboardexercice.h \
    qcmentryguess.h \
    appsettings.h \
    fntsetting.h \
    screensettings.h \
    symbolstables.h \
    tools.h

OTHER_FILES += \
    OtherFiles/*

FORMS += \
    mainwindow.ui \
    symbolsetting.ui \
    symbolsettingentry.ui \
    symbolsettingsection.ui \
    widgetwelcome.ui \
    qcmexercice.ui \
    keyboardexercice.ui \
    qcmentryguess.ui \
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

##################### install everything first then only deploy binary/target #####################
#INSTALLS += target everything thumbnail
#INSTALLS += target everythingButLibs thumbnail
#INSTALLS += everything thumbnail
INSTALLS += target

DISTFILES +=
