#-------------------------------------------------
# WORKING ON : - Handle duplicates in VocabDataPool atm it doesn't check for anything
#              - put date of launch in log file name so it doesn't overwrite the last one
#              - qpa, add support for Kobo Libra Colour
#              - qpa, add "default handler" in descriptor + legacytouchhandler parameter + rename experimentaltouchhandler into moderntouchhandler
#              - qpa, rewrite readme to make it more comprehensible that invertX, Y etc is only available on "legacyHandler"
#-------------------------------------------------
# TODO (0.3.2) : - Use fontMetrics::boundingRect to adapt VocabDisplayPage font size AND Qcm's stem
#                - Show last error's log in a popup on startup
#                - redo the Qmenu::right-arrow
#                - Investigate weird hard refresh (Edit page) glitch on Kobo Glo HD under unknown circumstances
#                - replace QList by QVector
#                - use qobject_cast instead of static_cast wherever it applies
#                - give users option to enable WorldWrap for QCM guesses (it looks fugly tho)
#                - investigate the size fixing bug if POPUP_FNT_GUESSES_RESIZE_ERROR_CNT is lower than 9
#                  (it's because of popup somehow)
#                - charging ascii symbol ⚡ expand the top bar => sizing is not fucked up but still ...
#-------------------------------------------------

TEMPLATE = subdirs
SUBDIRS = \
    koboplatformplugin \
    KoboExtraFunk \
    Obenkyobo

include(config.pri)

Obenkyobo.file = Src/Obenkyobo/Obenkyobo.pro
KoboExtraFunk.file = Src/Libs/KoboExtraFunk/KoboExtraFunk.pro
koboplatformplugin.file = Src/Libs/qt5-kobo-platform-plugin/koboplatformplugin.pro

KoboExtraFunk.depends = koboplatformplugin

Obenkyobo.depends = koboplatformplugin \
		    KoboExtraFunk

# "everything" contains ... everything including qt libs (=>slow deployment)
everything.path = /mnt/onboard/.adds
everything.files = $$files($${OUT_PWD}/Output/.adds/**)

# "everythingButQtLibs" contains ... everything bug qt libs (=>fast deployment)
everythingButQtLibs.path = /mnt/onboard/.adds
everythingButQtLibs.files = $$files($${OUT_PWD}/Output/.adds/**)
everythingButQtLibs.files -= $$files($${OUT_PWD}/Output/.adds/qt-linux-*)

# debug scripts, launcher and other scripts
scripts.path = /mnt/onboard/.adds/$${PROJECT_APP_NAME}
scripts.files = $$files($${OUT_PWD}/Output/.adds/$${PROJECT_APP_NAME}/*.sh)

##################### REMEMBER TO DEPLOY EVERYTHING THE FIRST TIME #####################
INSTALLS += everything
#INSTALLS += scripts

OTHER_FILES += \
    Readme.md \
    DevReadme.md \

CONFIG += ordered

