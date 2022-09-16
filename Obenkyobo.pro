#-------------------------------------------------
# WORKING ON :
#        - if first qcm is "Vocab -> Romanji to Vocab", Changing Stem size takes one qcm validation to apply
#        - "switch to ***" does not set guesses/stem to correct size and instead use the previous size
#        - add proper margins/spacing to qcm
#        - fix qcm 1x3, 2x3, 3x3 switch incorrect height
#        - Rework LearningScore() system so it can take a list of modification => otherwise at each VocabQCM validation we will modify copy the file twice
#        - restore correct value for POPUP_FNT_RESIZE_ERROR_CNT ( 3 )
#-------------------------------------------------
# TODO : - Finish sleep handling properly
#        - https://discord.com/channels/809205711778480158/958419944243089479/999974500756103188
#           - Anything below mk6 will not suspend while plugged in, yeah ;)
#           - also disconnecting charger / connecting also desuspends the device, which i will prevent ( as an optional feature )
#-------------------------------------------------
# TODO later (0.3.1) : - use a fontMetrics().boundingRect to determine max length of VocabExplorer entries text
#                      - Use fontMetrics::boundingRect to adapt VocabDisplayPage font size AND Qcm's stem
#                      - redo the Qmenu::right-arrow
#                      - Handle properly [UP_DIR] in VocabExplorerPage instead of adding a dummy entry
#                      - Investigate weird hard refresh (Edit page) glitch on Kobo Glo HD under unknown circumstances
#                      - replace QList by QVector
#                      - use qobject_cast instead of static_cast wherever it applies
#                      - give users option to enable WorldWrap for QCM guesses (it looks fugly tho)
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
    Src/Widgets/VocabFileEntryWidget.h \
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
