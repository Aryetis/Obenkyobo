#ifndef DEFINESLAND_H
#define DEFINESLAND_H

#define ENTRY_PER_ROW 3
#define MAX_LEARNING_STATE_VALUE 5
#define JP_SYMBOL_FNT_SIZE 20
#define DEFAULT_KANAS_ANSWER_RMJ_KANA_FNT_SIZE 45
#define DEFAULT_KANAS_ANSWER_KANA_RMJ_SIZE 45
#define DEFAULT_KANAS_STEM_FNT_SIZE 65
#define DEFAULT_VOCAB_ANSWER_RMJ_KANA_FNT_SIZE 15
#define DEFAULT_VOCAB_ANSWER_KANA_RMJ_SIZE 20
#define DEFAULT_VOCAB_STEM_FNT_SIZE 45
#define POPUP_BORDER_SIZE 5
#define POPUP_MARGIN_X 0.04f
#define POPUP_MARGIN_Y 0.02f
#define POPUP_WIDTH 0.9f
#define CORRECTED_FONT_PRECISION 1
#define CORRECTED_MINIMAL_FONT_SIZE 5
#define POPUP_FNT_GUESSES_RESIZE_ERROR_CNT 18 // if lower than 9, popup can be triggered during initial paint
                                              // and somehow it f**s up the whole sizing thing. TODO : investigate
#define POPUP_FNT_STEM_RESIZE_ERROR_CNT 3
#define POWER_REQUEST_TIMER 500 // ms
#define POST_WAKEUP_WIFI_DELAY 100 // ms
#define PRESLEEP_DURATION 60000 // ms
#define SET_AND_TRIM_LOOPING_RAILGUARD 25
#define VOCAB_FILE_ENTRY_TITLE_WIDTH_PCT 0.84f
#define VOCAB_FILE_ENTRY_TITLE_HEIGHT GetMy::Instance().Descriptor().height/20

enum KanaFamilyEnum
{
    hiragana,
    katakana
};

enum StemTypeEnum
{
    hiraganaStem,
    katakanaStem,
    romanjiStem,
    kanjiStem
};

enum QcmExerciceType
{
    Hiragana_to_Romanji_MCQ,
    Romanji_to_Hiragana_MCQ,
    Hiragana_to_Romanji_Kbd,
    Katakana_to_Romanji_MCQ,
    Romanji_to_Katakana_MCQ,
    Katakana_to_Romanji_Kbd,
    Vocabulary_to_Romanji_MCQ,
    Romanji_to_Vocabulary_MCQ
};

enum DeviceState
{
    awake,
    fakeSleeping,
    asleep, // actually it's more like "awaking" cause asleep is set "as soon as possible when waking up/before doing anything"
    busy
};

enum DisplayLSEnum
{
    FilesAndDirs,
    Files,
    None
};

enum ScreenSaverSetting
{
    Off,
    OnEverywhere,
    OnExceptVocab
};

#endif // DEFINESLAND_H
