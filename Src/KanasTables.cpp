#include "Src/KanasTables.h"

//========================================== HIRAGANA ==========================================
KanasTableFamily KanasTables::HiraganaSymbolsTableFamily
(
    "Hiragana",
    std::vector<SymbolsTableSection>
    {
        SymbolsTableSection
        (
            "Gojūon",
            std::vector<Kana>
            {
                Kana{"a", QStringLiteral("あ")},
                Kana{"i", QStringLiteral("い")},
                Kana{"u", QStringLiteral("う")},
                Kana{"e", QStringLiteral("え")},
                Kana{"o", QStringLiteral("お")},

                Kana{"ka", QStringLiteral("か")},
                Kana{"ki", QStringLiteral("き")},
                Kana{"ku", QStringLiteral("く")},
                Kana{"ke", QStringLiteral("け")},
                Kana{"ko", QStringLiteral("こ")},


                Kana{"sa", QStringLiteral("さ")},
                Kana{"shi", QStringLiteral("し")},
                Kana{"su", QStringLiteral("す")},
                Kana{"se", QStringLiteral("せ")},
                Kana{"so", QStringLiteral("そ")},

                Kana{"ta", QStringLiteral("た")},
                Kana{"chi", QStringLiteral("ち")},
                Kana{"tsu", QStringLiteral("つ")},
                Kana{"te", QStringLiteral("て")},
                Kana{"to", QStringLiteral("と")},

                Kana{"na", QStringLiteral("な")},
                Kana{"ni", QStringLiteral("に")},
                Kana{"nu", QStringLiteral("ぬ")},
                Kana{"ne", QStringLiteral("ね")},
                Kana{"no", QStringLiteral("の")},

                Kana{"ha", QStringLiteral("は")},
                Kana{"hi", QStringLiteral("ひ")},
                Kana{"fu", QStringLiteral("ふ")},
                Kana{"he", QStringLiteral("へ")},
                Kana{"ho", QStringLiteral("ほ")},

                Kana{"ma", QStringLiteral("ま")},
                Kana{"mi", QStringLiteral("み")},
                Kana{"mu", QStringLiteral("む")},
                Kana{"me", QStringLiteral("め")},
                Kana{"mo", QStringLiteral("も")},

                Kana{"ya", QStringLiteral("や")},
                Kana{"yu", QStringLiteral("ゆ")},
                Kana{"yo", QStringLiteral("よ")},

                Kana{"ra", QStringLiteral("ら")},
                Kana{"ri", QStringLiteral("り")},
                Kana{"ru", QStringLiteral("る")},
                Kana{"re", QStringLiteral("れ")},
                Kana{"ro", QStringLiteral("ろ")},

                Kana{"wa", QStringLiteral("わ")},
                Kana{"(w)o", QStringLiteral("を")},
                Kana{"n", QStringLiteral("ん")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Gojūon with (han)dakuten",
            std::vector<Kana>
            {
                Kana{"ga", QStringLiteral("が")},
                Kana{"gi", QStringLiteral("ぎ")},
                Kana{"gu", QStringLiteral("ぐ")},
                Kana{"ge", QStringLiteral("げ")},
                Kana{"go", QStringLiteral("ご")},

                Kana{"za", QStringLiteral("ざ")},
                Kana{"zi", QStringLiteral("じ")},
                Kana{"zu", QStringLiteral("ず")},
                Kana{"ze", QStringLiteral("ぜ")},
                Kana{"zo", QStringLiteral("ぞ")},

                Kana{"da", QStringLiteral("だ")},
                Kana{"ji", QStringLiteral("ぢ")},
                Kana{"(d)zu", QStringLiteral("づ")},
                Kana{"de", QStringLiteral("で")},
                Kana{"do", QStringLiteral("ど")},

                Kana{"ba", QStringLiteral("ば")},
                Kana{"bi", QStringLiteral("び")},
                Kana{"bu", QStringLiteral("ぶ")},
                Kana{"be", QStringLiteral("べ")},
                Kana{"bo", QStringLiteral("ぼ")},

                Kana{"pa", QStringLiteral("ぱ")},
                Kana{"pi", QStringLiteral("ぴ")},
                Kana{"pu", QStringLiteral("ぷ")},
                Kana{"pe", QStringLiteral("ぺ")},
                Kana{"po", QStringLiteral("ぽ")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Yōon",
            std::vector<Kana>
            {
                Kana{"kya", QStringLiteral("きゃ")},
                Kana{"kyu", QStringLiteral("きゅ")},
                Kana{"kyo", QStringLiteral("きょ")},

                Kana{"sha", QStringLiteral("しゃ")},
                Kana{"shu", QStringLiteral("しゅ")},
                Kana{"sho", QStringLiteral("しょ")},

                Kana{"cha", QStringLiteral("ちゃ")},
                Kana{"chu", QStringLiteral("ちゅ")},
                Kana{"cho", QStringLiteral("ちょ")},

                Kana{"nya", QStringLiteral("にゃ")},
                Kana{"nyu", QStringLiteral("にゅ")},
                Kana{"nyo", QStringLiteral("にょ")},

                Kana{"hya", QStringLiteral("ひゃ")},
                Kana{"hyu", QStringLiteral("ひゅ")},
                Kana{"hyo", QStringLiteral("ひょ")},

                Kana{"mya", QStringLiteral("みゃ")},
                Kana{"myu", QStringLiteral("みゅ")},
                Kana{"myo", QStringLiteral("みょ")},

                Kana{"rya", QStringLiteral("りゃ")},
                Kana{"ryu", QStringLiteral("りゅ")},
                Kana{"ryo", QStringLiteral("りょ")}
            },
            3
        ),

        SymbolsTableSection
        (
            "Yōon with (han)dakuten",
            std::vector<Kana>
            {
                Kana{"gya", QStringLiteral("ぎゃ")},
                Kana{"gyu", QStringLiteral("ぎゅ")},
                Kana{"gyo", QStringLiteral("ぎょ")},

                Kana{"zya/ja", QStringLiteral("じゃ")},
                Kana{"zyu/ju", QStringLiteral("じゅ")},
                Kana{"zyo/jo", QStringLiteral("じょ")},

                Kana{"dya/ja", QStringLiteral("ぢゃ")},
                Kana{"dyu/ju", QStringLiteral("ぢゅ")},
                Kana{"dyo/jo", QStringLiteral("ぢょ")},

                Kana{"bya", QStringLiteral("びゃ")},
                Kana{"byu", QStringLiteral("びゅ")},
                Kana{"byo", QStringLiteral("びょ")},

                Kana{"pya", QStringLiteral("ぴゃ")},
                Kana{"pyu", QStringLiteral("ぴゅ")},
                Kana{"pyo", QStringLiteral("ぴょ")}
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
            std::vector<Kana>
            {
                Kana{"a", QStringLiteral("ア")},
                Kana{"i", QStringLiteral("イ")},
                Kana{"u", QStringLiteral("ウ")},
                Kana{"e", QStringLiteral("エ")},
                Kana{"o", QStringLiteral("オ")},

                Kana{"ka", QStringLiteral("カ")},
                Kana{"ki", QStringLiteral("キ")},
                Kana{"ku", QStringLiteral("ク")},
                Kana{"ke", QStringLiteral("ケ")},
                Kana{"ko", QStringLiteral("コ")},


                Kana{"sa", QStringLiteral("サ")},
                Kana{"shi", QStringLiteral("シ")},
                Kana{"su", QStringLiteral("ス")},
                Kana{"se", QStringLiteral("セ")},
                Kana{"so", QStringLiteral("ソ")},

                Kana{"ta", QStringLiteral("タ")},
                Kana{"chi", QStringLiteral("チ")},
                Kana{"tsu", QStringLiteral("ツ")},
                Kana{"te", QStringLiteral("テ")},
                Kana{"to", QStringLiteral("ト")},

                Kana{"na", QStringLiteral("ナ")},
                Kana{"ni", QStringLiteral("ニ")},
                Kana{"nu", QStringLiteral("ヌ")},
                Kana{"ne", QStringLiteral("ネ")},
                Kana{"no", QStringLiteral("ノ")},

                Kana{"ha", QStringLiteral("ハ")},
                Kana{"hi", QStringLiteral("ヒ")},
                Kana{"fu", QStringLiteral("フ")},
                Kana{"he", QStringLiteral("ヘ")},
                Kana{"ho", QStringLiteral("ホ")},

                Kana{"ma", QStringLiteral("マ")},
                Kana{"mi", QStringLiteral("ミ")},
                Kana{"mu", QStringLiteral("ム")},
                Kana{"me", QStringLiteral("メ")},
                Kana{"mo", QStringLiteral("モ")},

                Kana{"ya", QStringLiteral("ヤ")},
                Kana{"yu", QStringLiteral("ユ")},
                Kana{"yo", QStringLiteral("ヨ")},

                Kana{"ra", QStringLiteral("ラ")},
                Kana{"ri", QStringLiteral("リ")},
                Kana{"ru", QStringLiteral("ル")},
                Kana{"re", QStringLiteral("レ")},
                Kana{"ro", QStringLiteral("ロ")},

                Kana{"wa", QStringLiteral("ワ")},
                Kana{"(w)o", QStringLiteral("ヲ")},
                Kana{"n", QStringLiteral("ン")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Gojūon with (han)dakuten",
            std::vector<Kana>
            {
                Kana{"ga", QStringLiteral("ガ")},
                Kana{"gi", QStringLiteral("ギ")},
                Kana{"gu", QStringLiteral("グ")},
                Kana{"ge", QStringLiteral("ゲ")},
                Kana{"go", QStringLiteral("ゴ")},

                Kana{"za", QStringLiteral("ザ")},
                Kana{"zi", QStringLiteral("ジ")},
                Kana{"zu", QStringLiteral("ズ")},
                Kana{"ze", QStringLiteral("ゼ")},
                Kana{"zo", QStringLiteral("ゾ")},

                Kana{"da", QStringLiteral("ダ")},
                Kana{"ji", QStringLiteral("ヂ")},
                Kana{"(d)zu", QStringLiteral("ヅ")},
                Kana{"de", QStringLiteral("デ")},
                Kana{"do", QStringLiteral("ド")},

                Kana{"ba", QStringLiteral("バ")},
                Kana{"bi", QStringLiteral("ビ")},
                Kana{"bu", QStringLiteral("ブ")},
                Kana{"be", QStringLiteral("ベ")},
                Kana{"bo", QStringLiteral("ボ")},

                Kana{"pa", QStringLiteral("パ")},
                Kana{"pi", QStringLiteral("ピ")},
                Kana{"pu", QStringLiteral("プ")},
                Kana{"pe", QStringLiteral("ペ")},
                Kana{"po", QStringLiteral("ポ")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Yōon",
            std::vector<Kana>
            {
                Kana{"kya", QStringLiteral("キャ")},
                Kana{"kyu", QStringLiteral("キュ")},
                Kana{"kyo", QStringLiteral("キョ")},

                Kana{"sha", QStringLiteral("シャ")},
                Kana{"shu", QStringLiteral("シュ")},
                Kana{"sho", QStringLiteral("ショ")},

                Kana{"cha", QStringLiteral("チャ")},
                Kana{"chu", QStringLiteral("チュ")},
                Kana{"cho", QStringLiteral("チョ")},

                Kana{"nya", QStringLiteral("ニャ")},
                Kana{"nyu", QStringLiteral("ニュ")},
                Kana{"nyo", QStringLiteral("ニョ")},

                Kana{"hya", QStringLiteral("ヒャ")},
                Kana{"hyu", QStringLiteral("ヒュ")},
                Kana{"hyo", QStringLiteral("ヒョ")},

                Kana{"mya", QStringLiteral("ミャ")},
                Kana{"myu", QStringLiteral("ミュ")},
                Kana{"myo", QStringLiteral("ミョ")},

                Kana{"rya", QStringLiteral("リャ")},
                Kana{"ryu", QStringLiteral("リュ")},
                Kana{"ryo", QStringLiteral("リョ")},
            },
            3
        ),

        SymbolsTableSection
        (
            "Yōon with (han)dakuten",
            std::vector<Kana>
            {
                Kana{"gya", QStringLiteral("ギャ")},
                Kana{"gyu", QStringLiteral("ギュ")},
                Kana{"gyo", QStringLiteral("ギョ")},

                Kana{"zya/ja", QStringLiteral("ジャ")},
                Kana{"zyu/ju", QStringLiteral("ジュ")},
                Kana{"zyo/jo", QStringLiteral("ジョ")},

                Kana{"dya/ja", QStringLiteral("ヂャ")},
                Kana{"dyu/ju", QStringLiteral("ヂュ")},
                Kana{"dyo/jo", QStringLiteral("ヂョ")},

                Kana{"bya", QStringLiteral("ビャ")},
                Kana{"byu", QStringLiteral("ビュ")},
                Kana{"byo", QStringLiteral("ビョ")},

                Kana{"pya", QStringLiteral("ピャ")},
                Kana{"pyu", QStringLiteral("ピュ")},
                Kana{"pyo", QStringLiteral("ピョ")}
            },
            3
        )
    }
);
