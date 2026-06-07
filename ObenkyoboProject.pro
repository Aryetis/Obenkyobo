#-------------------------------------------------
# WORKING ON : - Handle duplicates in VocabDataPool atm it doesn't check for anything
#              - put date of launch in log file name so it doesn't overwrite the last one
#              - fix jpg lib
#              - fix sleep not ignoring button input(can scroll in note when "sleeping")
#              - qpa, rewrite readme to make it more comprehensible that invertX, Y etc is only available on "legacyHandler"
#              - write a proper ExtraFunk manual
#              - added IsColor to KoboExtraFunk
#   fix everything, debug libkobo not working anymore with export QT_DEBUG_PLUGINS=1
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
#                - Fix wifi icon not set to "partial checking" when waking up
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

##################################################
# Adding stuff to INSTALLS for easier deployment #
##################################################

###########################################
# qt libs and stuff
# This used to work on WSL... not anymore on dockerized debian... because qt
#qt.path = /mnt/onboard/.adds/qt-linux-5.15-kde-kobo
#qt.files = $$files($${OUT_PWD}/Output/.adds/qt-linux-5.15-kde-kobo/**, true)
###########################################
# This works but takes around 10 minutes
#dir = $$OUT_PWD/Output/.adds/
#currentFiles = $$files($$dir/*, true)
#foo = 1
#
#for(file, currentFiles) {
#    result = $$system(test -d $$file && echo folder_detected)
#    equals(result, folder_detected) {
#        next())
#    }
#    #message($$file)
#    #message(/mnt/onboard/.adds/$$relative_path($$file, $$dir))
#
#    bar$${foo}.files = $$file
#    bar$${foo}.path = /mnt/onboard/.adds/$$relative_path($$file, $$dir)
#    INSTALLS += bar$${foo}
#
#    foo = $$num_add($$foo, 1)
#}
###########################################
# fuck it, just rsync the whole thing "by hand" in Obenkyobo.pro's QMAKE_POST_LINK

###########################################
# kfmon
kfmon.path = /mnt/onboard/.adds/kfmon/config/
kfmon.files = $$files($${OUT_PWD}/Output/.adds/kfmon/config/$${PROJECT_APP_NAME}.ini)

###########################################
# nm
nm.path = /mnt/onboard/.adds/nm/
nm.files = $$files($${OUT_PWD}/Output/.adds/nm/$${PROJECT_APP_NAME})

###########################################
# debug scripts, launcher and other scripts
scripts.path = /mnt/onboard/.adds/$${PROJECT_APP_NAME}
scripts.files = $$files($${OUT_PWD}/Output/.adds/$${PROJECT_APP_NAME}/*.sh)

###########################################
# notes
notes.path = /mnt/onboard/.adds/$${PROJECT_APP_NAME}/notes
notes.files = $$files($${OUT_PWD}/Output/.adds/$${PROJECT_APP_NAME}/notes/*.md)
notes.files += $$files($${OUT_PWD}/Output/.adds/$${PROJECT_APP_NAME}/notes/*.txt)

###########################################
# vocab
vocab.path = /mnt/onboard/.adds/$${PROJECT_APP_NAME}/vocab
vocab.files = $$files($${OUT_PWD}/Output/.adds/$${PROJECT_APP_NAME}/vocab/*.oben)

###########################################
#INSTALLS = # leave empty to only send Obenkyobo, KoboPlatformPlugin and KoboExtraFunk
# INSTALLS += kfmon \
#             nm \
#             scripts \
#             notes \
#             vocab \

OTHER_FILES += \
    Readme.md \
    DevReadme.md \

CONFIG += ordered