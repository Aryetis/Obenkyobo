#-------------------------------------------------
#
# TODO : - guess hiragana / katakana from romanji mode (4 / 6 / 8 / 10 choices)
#        - guess romanji from hiragana / katakana mode
#        - custom selection mode
#        - 3 subcategories for hiragana / katakana tenten
#        - handakuten / dakuten mode / etc
#        - weighted random list
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
        mainwindow.cpp

RESOURCES += \
        Resources/fonts.qrc \

HEADERS += \
        mainwindow.h

OTHER_FILES += \
        OtherFiles/*

FORMS += \
        mainwindow.ui

# contains everything (use target to only ship binary)
customTarget.path = /mnt/onboard/.adds
customTarget.files = $$files($${OUT_PWD}/Output/.adds/**)

# just the thumbnail
thumbnail.path = /mnt/onboard/
thumbnail.files += $$files($${OUT_PWD}/Output/$${TARGET}.png)

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /mnt/onboard/.adds/$${TARGET}
!isEmpty(target.path):
#INSTALLS += customTarget thumbnail
INSTALLS += target thumbnail
