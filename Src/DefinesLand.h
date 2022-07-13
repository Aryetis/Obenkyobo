#ifndef DEFINESLAND_H
#define DEFINESLAND_H

// How long before I hate this ?

#define ENTRY_PER_ROW 3
#define MAX_LEARNING_STATE_VALUE 5
#define JP_SYMBOL_FNT_SIZE 20
#define DEFAULT_ANSWER_RMJ_KANA_FNT_SIZE 25
#define DEFAULT_ANSWER_KANA_RMJ_SIZE 50
#define DEFAULT_STEM_FNT_SIZE 70
#define POPUP_BORDER_SIZE 5
#define POPUP_MARGIN_X 0.04f
#define POPUP_MARGIN_Y 0.02f
#define POPUP_WIDTH 0.9f

enum KanaFamilyEnum
{
    hiragana,
    katakana
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

enum QcmTypeEnum
{
    KanaToRmj,
    RmjToKana
};

enum DeviceState
{
    asleep,
    awake,
    busy
};

#endif // DEFINESLAND_H
