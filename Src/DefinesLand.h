#ifndef DEFINESLAND_H
#define DEFINESLAND_H

#define ENTRY_PER_ROW 3
#define MAX_LEARNING_STATE_VALUE 5
#define JP_SYMBOL_FNT_SIZE 20
#define DEFAULT_KANAS_ANSWER_RMJ_KANA_FNT_SIZE 45
#define DEFAULT_KANAS_ANSWER_KANA_RMJ_SIZE 45
#define DEFAULT_KANAS_STEM_FNT_SIZE 70
#define DEFAULT_KANAS_ALLOW_SILENT_RESIZE false
#define DEFAULT_VOCAB_ANSWER_RMJ_KANA_FNT_SIZE 30
#define DEFAULT_VOCAB_ANSWER_KANA_RMJ_SIZE 30
#define DEFAULT_VOCAB_STEM_FNT_SIZE 45
#define DEFAULT_VOCAB_ALLOW_SILENT_RESIZE true
#define POPUP_BORDER_SIZE 5
#define POPUP_MARGIN_X 0.04f
#define POPUP_MARGIN_Y 0.02f
#define POPUP_WIDTH 0.9f
#define CORRECTED_FONT_PRECISION 1
#define CORRECTED_MINIMAL_FONT_SIZE 5

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

enum DeviceState
{
    asleep,
    awake,
    busy
};

enum DisplayLSEnum
{
    FilesAndDirs,
    Files,
    None
};

#endif // DEFINESLAND_H
