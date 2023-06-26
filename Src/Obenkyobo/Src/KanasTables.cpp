#include "Src/KanasTables.h"
#include "Src/DefinesLand.h"
#include "Src/QcmDataEntryKana.h"

//========================================== HIRAGANA ==========================================
KanasTableFamily KanasTables::HiraganaSymbolsTableFamily
(
    "Hiragana",
    std::vector<SymbolsTableSection>
    {
        SymbolsTableSection
        (
            "Gojūon",
            std::vector<QcmDataEntryKana>
            {
                QcmDataEntryKana{"a", QStringLiteral("あ")},
                QcmDataEntryKana{"i", QStringLiteral("い")},
                QcmDataEntryKana{"u", QStringLiteral("う")},
                QcmDataEntryKana{"e", QStringLiteral("え")},
                QcmDataEntryKana{"o", QStringLiteral("お")},

                QcmDataEntryKana{"ka", QStringLiteral("か")},
                QcmDataEntryKana{"ki", QStringLiteral("き")},
                QcmDataEntryKana{"ku", QStringLiteral("く")},
                QcmDataEntryKana{"ke", QStringLiteral("け")},
                QcmDataEntryKana{"ko", QStringLiteral("こ")},


                QcmDataEntryKana{"sa", QStringLiteral("さ")},
                QcmDataEntryKana{"shi", QStringLiteral("し")},
                QcmDataEntryKana{"su", QStringLiteral("す")},
                QcmDataEntryKana{"se", QStringLiteral("せ")},
                QcmDataEntryKana{"so", QStringLiteral("そ")},

                QcmDataEntryKana{"ta", QStringLiteral("た")},
                QcmDataEntryKana{"chi", QStringLiteral("ち")},
                QcmDataEntryKana{"tsu", QStringLiteral("つ")},
                QcmDataEntryKana{"te", QStringLiteral("て")},
                QcmDataEntryKana{"to", QStringLiteral("と")},

                QcmDataEntryKana{"na", QStringLiteral("な")},
                QcmDataEntryKana{"ni", QStringLiteral("に")},
                QcmDataEntryKana{"nu", QStringLiteral("ぬ")},
                QcmDataEntryKana{"ne", QStringLiteral("ね")},
                QcmDataEntryKana{"no", QStringLiteral("の")},

                QcmDataEntryKana{"ha", QStringLiteral("は")},
                QcmDataEntryKana{"hi", QStringLiteral("ひ")},
                QcmDataEntryKana{"fu", QStringLiteral("ふ")},
                QcmDataEntryKana{"he", QStringLiteral("へ")},
                QcmDataEntryKana{"ho", QStringLiteral("ほ")},

                QcmDataEntryKana{"ma", QStringLiteral("ま")},
                QcmDataEntryKana{"mi", QStringLiteral("み")},
                QcmDataEntryKana{"mu", QStringLiteral("む")},
                QcmDataEntryKana{"me", QStringLiteral("め")},
                QcmDataEntryKana{"mo", QStringLiteral("も")},

                QcmDataEntryKana{"ya", QStringLiteral("や")},
                QcmDataEntryKana{"yu", QStringLiteral("ゆ")},
                QcmDataEntryKana{"yo", QStringLiteral("よ")},

                QcmDataEntryKana{"ra", QStringLiteral("ら")},
                QcmDataEntryKana{"ri", QStringLiteral("り")},
                QcmDataEntryKana{"ru", QStringLiteral("る")},
                QcmDataEntryKana{"re", QStringLiteral("れ")},
                QcmDataEntryKana{"ro", QStringLiteral("ろ")},

                QcmDataEntryKana{"wa", QStringLiteral("わ")},
                QcmDataEntryKana{"(w)o", QStringLiteral("を")},
                QcmDataEntryKana{"n", QStringLiteral("ん")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Gojūon with (han)dakuten",
            std::vector<QcmDataEntryKana>
            {
                QcmDataEntryKana{"ga", QStringLiteral("が")},
                QcmDataEntryKana{"gi", QStringLiteral("ぎ")},
                QcmDataEntryKana{"gu", QStringLiteral("ぐ")},
                QcmDataEntryKana{"ge", QStringLiteral("げ")},
                QcmDataEntryKana{"go", QStringLiteral("ご")},

                QcmDataEntryKana{"za", QStringLiteral("ざ")},
                QcmDataEntryKana{"zi", QStringLiteral("じ")},
                QcmDataEntryKana{"zu", QStringLiteral("ず")},
                QcmDataEntryKana{"ze", QStringLiteral("ぜ")},
                QcmDataEntryKana{"zo", QStringLiteral("ぞ")},

                QcmDataEntryKana{"da", QStringLiteral("だ")},
                QcmDataEntryKana{"ji", QStringLiteral("ぢ")},
                QcmDataEntryKana{"(d)zu", QStringLiteral("づ")},
                QcmDataEntryKana{"de", QStringLiteral("で")},
                QcmDataEntryKana{"do", QStringLiteral("ど")},

                QcmDataEntryKana{"ba", QStringLiteral("ば")},
                QcmDataEntryKana{"bi", QStringLiteral("び")},
                QcmDataEntryKana{"bu", QStringLiteral("ぶ")},
                QcmDataEntryKana{"be", QStringLiteral("べ")},
                QcmDataEntryKana{"bo", QStringLiteral("ぼ")},

                QcmDataEntryKana{"pa", QStringLiteral("ぱ")},
                QcmDataEntryKana{"pi", QStringLiteral("ぴ")},
                QcmDataEntryKana{"pu", QStringLiteral("ぷ")},
                QcmDataEntryKana{"pe", QStringLiteral("ぺ")},
                QcmDataEntryKana{"po", QStringLiteral("ぽ")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Yōon",
            std::vector<QcmDataEntryKana>
            {
                QcmDataEntryKana{"kya", QStringLiteral("きゃ")},
                QcmDataEntryKana{"kyu", QStringLiteral("きゅ")},
                QcmDataEntryKana{"kyo", QStringLiteral("きょ")},

                QcmDataEntryKana{"sha", QStringLiteral("しゃ")},
                QcmDataEntryKana{"shu", QStringLiteral("しゅ")},
                QcmDataEntryKana{"sho", QStringLiteral("しょ")},

                QcmDataEntryKana{"cha", QStringLiteral("ちゃ")},
                QcmDataEntryKana{"chu", QStringLiteral("ちゅ")},
                QcmDataEntryKana{"cho", QStringLiteral("ちょ")},

                QcmDataEntryKana{"nya", QStringLiteral("にゃ")},
                QcmDataEntryKana{"nyu", QStringLiteral("にゅ")},
                QcmDataEntryKana{"nyo", QStringLiteral("にょ")},

                QcmDataEntryKana{"hya", QStringLiteral("ひゃ")},
                QcmDataEntryKana{"hyu", QStringLiteral("ひゅ")},
                QcmDataEntryKana{"hyo", QStringLiteral("ひょ")},

                QcmDataEntryKana{"mya", QStringLiteral("みゃ")},
                QcmDataEntryKana{"myu", QStringLiteral("みゅ")},
                QcmDataEntryKana{"myo", QStringLiteral("みょ")},

                QcmDataEntryKana{"rya", QStringLiteral("りゃ")},
                QcmDataEntryKana{"ryu", QStringLiteral("りゅ")},
                QcmDataEntryKana{"ryo", QStringLiteral("りょ")}
            },
            3
        ),

        SymbolsTableSection
        (
            "Yōon with (han)dakuten",
            std::vector<QcmDataEntryKana>
            {
                QcmDataEntryKana{"gya", QStringLiteral("ぎゃ")},
                QcmDataEntryKana{"gyu", QStringLiteral("ぎゅ")},
                QcmDataEntryKana{"gyo", QStringLiteral("ぎょ")},

                QcmDataEntryKana{"zya/ja", QStringLiteral("じゃ")},
                QcmDataEntryKana{"zyu/ju", QStringLiteral("じゅ")},
                QcmDataEntryKana{"zyo/jo", QStringLiteral("じょ")},

                QcmDataEntryKana{"dya/ja", QStringLiteral("ぢゃ")},
                QcmDataEntryKana{"dyu/ju", QStringLiteral("ぢゅ")},
                QcmDataEntryKana{"dyo/jo", QStringLiteral("ぢょ")},

                QcmDataEntryKana{"bya", QStringLiteral("びゃ")},
                QcmDataEntryKana{"byu", QStringLiteral("びゅ")},
                QcmDataEntryKana{"byo", QStringLiteral("びょ")},

                QcmDataEntryKana{"pya", QStringLiteral("ぴゃ")},
                QcmDataEntryKana{"pyu", QStringLiteral("ぴゅ")},
                QcmDataEntryKana{"pyo", QStringLiteral("ぴょ")}
            },
            3
        )
    }
);

//========================================== KATAKANA ==========================================
KanasTableFamily KanasTables::KatakanaSymbolsTableFamily
(
    "Katakana",
    std::vector<SymbolsTableSection>
    {
        SymbolsTableSection
        (
            "Gojūon",
            std::vector<QcmDataEntryKana>
            {
                QcmDataEntryKana{"a", QStringLiteral("ア")},
                QcmDataEntryKana{"i", QStringLiteral("イ")},
                QcmDataEntryKana{"u", QStringLiteral("ウ")},
                QcmDataEntryKana{"e", QStringLiteral("エ")},
                QcmDataEntryKana{"o", QStringLiteral("オ")},

                QcmDataEntryKana{"ka", QStringLiteral("カ")},
                QcmDataEntryKana{"ki", QStringLiteral("キ")},
                QcmDataEntryKana{"ku", QStringLiteral("ク")},
                QcmDataEntryKana{"ke", QStringLiteral("ケ")},
                QcmDataEntryKana{"ko", QStringLiteral("コ")},


                QcmDataEntryKana{"sa", QStringLiteral("サ")},
                QcmDataEntryKana{"shi", QStringLiteral("シ")},
                QcmDataEntryKana{"su", QStringLiteral("ス")},
                QcmDataEntryKana{"se", QStringLiteral("セ")},
                QcmDataEntryKana{"so", QStringLiteral("ソ")},

                QcmDataEntryKana{"ta", QStringLiteral("タ")},
                QcmDataEntryKana{"chi", QStringLiteral("チ")},
                QcmDataEntryKana{"tsu", QStringLiteral("ツ")},
                QcmDataEntryKana{"te", QStringLiteral("テ")},
                QcmDataEntryKana{"to", QStringLiteral("ト")},

                QcmDataEntryKana{"na", QStringLiteral("ナ")},
                QcmDataEntryKana{"ni", QStringLiteral("ニ")},
                QcmDataEntryKana{"nu", QStringLiteral("ヌ")},
                QcmDataEntryKana{"ne", QStringLiteral("ネ")},
                QcmDataEntryKana{"no", QStringLiteral("ノ")},

                QcmDataEntryKana{"ha", QStringLiteral("ハ")},
                QcmDataEntryKana{"hi", QStringLiteral("ヒ")},
                QcmDataEntryKana{"fu", QStringLiteral("フ")},
                QcmDataEntryKana{"he", QStringLiteral("ヘ")},
                QcmDataEntryKana{"ho", QStringLiteral("ホ")},

                QcmDataEntryKana{"ma", QStringLiteral("マ")},
                QcmDataEntryKana{"mi", QStringLiteral("ミ")},
                QcmDataEntryKana{"mu", QStringLiteral("ム")},
                QcmDataEntryKana{"me", QStringLiteral("メ")},
                QcmDataEntryKana{"mo", QStringLiteral("モ")},

                QcmDataEntryKana{"ya", QStringLiteral("ヤ")},
                QcmDataEntryKana{"yu", QStringLiteral("ユ")},
                QcmDataEntryKana{"yo", QStringLiteral("ヨ")},

                QcmDataEntryKana{"ra", QStringLiteral("ラ")},
                QcmDataEntryKana{"ri", QStringLiteral("リ")},
                QcmDataEntryKana{"ru", QStringLiteral("ル")},
                QcmDataEntryKana{"re", QStringLiteral("レ")},
                QcmDataEntryKana{"ro", QStringLiteral("ロ")},

                QcmDataEntryKana{"wa", QStringLiteral("ワ")},
                QcmDataEntryKana{"(w)o", QStringLiteral("ヲ")},
                QcmDataEntryKana{"n", QStringLiteral("ン")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Gojūon with (han)dakuten",
            std::vector<QcmDataEntryKana>
            {
                QcmDataEntryKana{"ga", QStringLiteral("ガ")},
                QcmDataEntryKana{"gi", QStringLiteral("ギ")},
                QcmDataEntryKana{"gu", QStringLiteral("グ")},
                QcmDataEntryKana{"ge", QStringLiteral("ゲ")},
                QcmDataEntryKana{"go", QStringLiteral("ゴ")},

                QcmDataEntryKana{"za", QStringLiteral("ザ")},
                QcmDataEntryKana{"zi", QStringLiteral("ジ")},
                QcmDataEntryKana{"zu", QStringLiteral("ズ")},
                QcmDataEntryKana{"ze", QStringLiteral("ゼ")},
                QcmDataEntryKana{"zo", QStringLiteral("ゾ")},

                QcmDataEntryKana{"da", QStringLiteral("ダ")},
                QcmDataEntryKana{"ji", QStringLiteral("ヂ")},
                QcmDataEntryKana{"(d)zu", QStringLiteral("ヅ")},
                QcmDataEntryKana{"de", QStringLiteral("デ")},
                QcmDataEntryKana{"do", QStringLiteral("ド")},

                QcmDataEntryKana{"ba", QStringLiteral("バ")},
                QcmDataEntryKana{"bi", QStringLiteral("ビ")},
                QcmDataEntryKana{"bu", QStringLiteral("ブ")},
                QcmDataEntryKana{"be", QStringLiteral("ベ")},
                QcmDataEntryKana{"bo", QStringLiteral("ボ")},

                QcmDataEntryKana{"pa", QStringLiteral("パ")},
                QcmDataEntryKana{"pi", QStringLiteral("ピ")},
                QcmDataEntryKana{"pu", QStringLiteral("プ")},
                QcmDataEntryKana{"pe", QStringLiteral("ペ")},
                QcmDataEntryKana{"po", QStringLiteral("ポ")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Yōon",
            std::vector<QcmDataEntryKana>
            {
                QcmDataEntryKana{"kya", QStringLiteral("キャ")},
                QcmDataEntryKana{"kyu", QStringLiteral("キュ")},
                QcmDataEntryKana{"kyo", QStringLiteral("キョ")},

                QcmDataEntryKana{"sha", QStringLiteral("シャ")},
                QcmDataEntryKana{"shu", QStringLiteral("シュ")},
                QcmDataEntryKana{"sho", QStringLiteral("ショ")},

                QcmDataEntryKana{"cha", QStringLiteral("チャ")},
                QcmDataEntryKana{"chu", QStringLiteral("チュ")},
                QcmDataEntryKana{"cho", QStringLiteral("チョ")},

                QcmDataEntryKana{"nya", QStringLiteral("ニャ")},
                QcmDataEntryKana{"nyu", QStringLiteral("ニュ")},
                QcmDataEntryKana{"nyo", QStringLiteral("ニョ")},

                QcmDataEntryKana{"hya", QStringLiteral("ヒャ")},
                QcmDataEntryKana{"hyu", QStringLiteral("ヒュ")},
                QcmDataEntryKana{"hyo", QStringLiteral("ヒョ")},

                QcmDataEntryKana{"mya", QStringLiteral("ミャ")},
                QcmDataEntryKana{"myu", QStringLiteral("ミュ")},
                QcmDataEntryKana{"myo", QStringLiteral("ミョ")},

                QcmDataEntryKana{"rya", QStringLiteral("リャ")},
                QcmDataEntryKana{"ryu", QStringLiteral("リュ")},
                QcmDataEntryKana{"ryo", QStringLiteral("リョ")},
            },
            3
        ),

        SymbolsTableSection
        (
            "Yōon with (han)dakuten",
            std::vector<QcmDataEntryKana>
            {
                QcmDataEntryKana{"gya", QStringLiteral("ギャ")},
                QcmDataEntryKana{"gyu", QStringLiteral("ギュ")},
                QcmDataEntryKana{"gyo", QStringLiteral("ギョ")},

                QcmDataEntryKana{"zya/ja", QStringLiteral("ジャ")},
                QcmDataEntryKana{"zyu/ju", QStringLiteral("ジュ")},
                QcmDataEntryKana{"zyo/jo", QStringLiteral("ジョ")},

                QcmDataEntryKana{"dya/ja", QStringLiteral("ヂャ")},
                QcmDataEntryKana{"dyu/ju", QStringLiteral("ヂュ")},
                QcmDataEntryKana{"dyo/jo", QStringLiteral("ヂョ")},

                QcmDataEntryKana{"bya", QStringLiteral("ビャ")},
                QcmDataEntryKana{"byu", QStringLiteral("ビュ")},
                QcmDataEntryKana{"byo", QStringLiteral("ビョ")},

                QcmDataEntryKana{"pya", QStringLiteral("ピャ")},
                QcmDataEntryKana{"pyu", QStringLiteral("ピュ")},
                QcmDataEntryKana{"pyo", QStringLiteral("ピョ")}
            },
            3
        )
    }
);

void KanasTableFamily::InitializeSerializedVals() // Can't do it in constructor because SettingSerializer isn't set yet.
{
    for (SymbolsTableSection& sts : data)
        for (QcmDataEntry& s : sts.Data())
            if (static_cast<QcmDataEntryKana&>(s).RegisterAndInitializeSerializedVals(name, this))
                ++nbrOfEnabled;
}

void KanasTableFamily::ResetWeights()
{
    for (SymbolsTableSection& sts : data)
        for (QcmDataEntry& s : sts.Data())
            s.LearningScore(MAX_LEARNING_STATE_VALUE);
}

