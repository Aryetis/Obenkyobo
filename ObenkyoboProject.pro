#-------------------------------------------------
# WORKING ON : - How do qcm handle double identical entries for kanjis ? should select the lower LS, make sure of it
#              - put date of launch in log file name so it doesn't overwrite the last one
#              - fix packager with new QtCreator 13 workflow...
#-------------------------------------------------
# TODO (0.3.1) : - Use fontMetrics::boundingRect to adapt VocabDisplayPage font size AND Qcm's stem
#                - Show last error's log in a popup on startup
#                - use a fontMetrics().boundingRect to determine max length of VocabExplorer entries text  
#                - redo the Qmenu::right-arrow
#                - Investigate weird hard refresh (Edit page) glitch on Kobo Glo HD under unknown circumstances
#                - replace QList by QVector
#                - use qobject_cast instead of static_cast wherever it applies
#                - give users option to enable WorldWrap for QCM guesses (it looks fugly tho)
#                - investigate the size fixing bug if POPUP_FNT_GUESSES_RESIZE_ERROR_CNT is lower than 9
#                  (it's because of popup somehow)
#                - charging ascii symbol ⚡ expand the top bar => sizing is not fucked up but still ...
#                - How did I end up with an entry "[fontType=hiragana][jp=ひ][kanji=火][trad=feu][learningScore=6]"
#                   ?!? maybe bad manipulation during some debug session ?
#-------------------------------------------------

TEMPLATE = subdirs
SUBDIRS = \
    koboplatformplugin \
    Obenkyobo

Obenkyobo.file = Src/Obenkyobo/Obenkyobo.pro
koboplatformplugin.file = Src/Libs/qt5-kobo-platform-plugin/koboplatformplugin.pro

Obenkyobo.depends = koboplatformplugin

CONFIG += ordered

