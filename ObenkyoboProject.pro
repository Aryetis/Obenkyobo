#-------------------------------------------------
# WORKING ON : - Finish moving on to git submodules properly and move the packager to its own project so my lazy ass won't have to set it up ever again
#              - Use fontMetrics::boundingRect to adapt VocabDisplayPage font size AND Qcm's stem
#              - Make [UP_DIR] checkbox tick everything in current dir
#              - Handle properly [UP_DIR] in VocabExplorerPage instead of adding a dummy entry
#-------------------------------------------------
# TODO (0.3.1) : - Show last error's log in a popup on startup
#                - use a fontMetrics().boundingRect to determine max length of VocabExplorer entries text  
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
    Obenkyobo \
#Packager

Obenkyobo.file = Src/Obenkyobo/Obenkyobo.pro
koboplatformplugin.file = Src/Libs/qt5-kobo-platform-plugin/koboplatformplugin.pro
#Packager.file = Src/Packager/Packager.pro # et pif paf pouf un call à system() pour appeler mon packager.sh directement là dedans

Obenkyobo.depends = koboplatformplugin

CONFIG += ordered

