#include "Src/symbolstables.h"

//========================================== HIRAGANA ==========================================
SymbolsTableFamily SymbolsTables::HiraganaSymbolsTableFamily
(
    "Hiragana",
    std::vector<SymbolsTableSection>
    {
        SymbolsTableSection
        (
            "Gojūon",
            std::vector<Symbol>
            {
                Symbol{"a", QStringLiteral("あ")},
                Symbol{"i", QStringLiteral("い")},
                Symbol{"u", QStringLiteral("う")},
                Symbol{"e", QStringLiteral("え")},
                Symbol{"o", QStringLiteral("お")},

                Symbol{"ka", QStringLiteral("か")},
                Symbol{"ki", QStringLiteral("き")},
                Symbol{"ku", QStringLiteral("く")},
                Symbol{"ke", QStringLiteral("け")},
                Symbol{"ko", QStringLiteral("こ")},


                Symbol{"sa", QStringLiteral("さ")},
                Symbol{"shi", QStringLiteral("し")},
                Symbol{"su", QStringLiteral("す")},
                Symbol{"se", QStringLiteral("せ")},
                Symbol{"so", QStringLiteral("そ")},

                Symbol{"ta", QStringLiteral("た")},
                Symbol{"chi", QStringLiteral("ち")},
                Symbol{"tsu", QStringLiteral("つ")},
                Symbol{"te", QStringLiteral("て")},
                Symbol{"to", QStringLiteral("と")},

                Symbol{"na", QStringLiteral("な")},
                Symbol{"ni", QStringLiteral("に")},
                Symbol{"nu", QStringLiteral("ぬ")},
                Symbol{"ne", QStringLiteral("ね")},
                Symbol{"no", QStringLiteral("の")},

                Symbol{"ha", QStringLiteral("は")},
                Symbol{"hi", QStringLiteral("ひ")},
                Symbol{"fu", QStringLiteral("ふ")},
                Symbol{"he", QStringLiteral("へ")},
                Symbol{"ho", QStringLiteral("ほ")},

                Symbol{"ma", QStringLiteral("ま")},
                Symbol{"mi", QStringLiteral("み")},
                Symbol{"mu", QStringLiteral("む")},
                Symbol{"me", QStringLiteral("め")},
                Symbol{"mo", QStringLiteral("も")},

                Symbol{"ya", QStringLiteral("や")},
                Symbol{"yu", QStringLiteral("ゆ")},
                Symbol{"yo", QStringLiteral("よ")},

                Symbol{"ra", QStringLiteral("ら")},
                Symbol{"ri", QStringLiteral("り")},
                Symbol{"ru", QStringLiteral("る")},
                Symbol{"re", QStringLiteral("れ")},
                Symbol{"ro", QStringLiteral("ろ")},

                Symbol{"wa", QStringLiteral("わ")},
                Symbol{"(w)o", QStringLiteral("を")},
                Symbol{"n", QStringLiteral("ん")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Gojūon with (han)dakuten",
            std::vector<Symbol>
            {
                Symbol{"ga", QStringLiteral("が")},
                Symbol{"gi", QStringLiteral("ぎ")},
                Symbol{"gu", QStringLiteral("ぐ")},
                Symbol{"ge", QStringLiteral("げ")},
                Symbol{"go", QStringLiteral("ご")},

                Symbol{"za", QStringLiteral("ざ")},
                Symbol{"zi", QStringLiteral("じ")},
                Symbol{"zu", QStringLiteral("ず")},
                Symbol{"ze", QStringLiteral("ぜ")},
                Symbol{"zo", QStringLiteral("ぞ")},

                Symbol{"da", QStringLiteral("だ")},
                Symbol{"ji", QStringLiteral("ぢ")},
                Symbol{"(d)zu", QStringLiteral("づ")},
                Symbol{"de", QStringLiteral("で")},
                Symbol{"do", QStringLiteral("ど")},

                Symbol{"ba", QStringLiteral("ば")},
                Symbol{"bi", QStringLiteral("び")},
                Symbol{"bu", QStringLiteral("ぶ")},
                Symbol{"be", QStringLiteral("べ")},
                Symbol{"bo", QStringLiteral("ぼ")},

                Symbol{"pa", QStringLiteral("ぱ")},
                Symbol{"pi", QStringLiteral("ぴ")},
                Symbol{"pu", QStringLiteral("ぷ")},
                Symbol{"pe", QStringLiteral("ぺ")},
                Symbol{"po", QStringLiteral("ぽ")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Yōon",
            std::vector<Symbol>
            {
                Symbol{"kya", QStringLiteral("きゃ")},
                Symbol{"kyu", QStringLiteral("きゅ")},
                Symbol{"kyo", QStringLiteral("きょ")},

                Symbol{"sha", QStringLiteral("しゃ")},
                Symbol{"shu", QStringLiteral("しゅ")},
                Symbol{"sho", QStringLiteral("しょ")},

                Symbol{"cha", QStringLiteral("ちゃ")},
                Symbol{"chu", QStringLiteral("ちゅ")},
                Symbol{"cho", QStringLiteral("ちょ")},

                Symbol{"nya", QStringLiteral("にゃ")},
                Symbol{"nyu", QStringLiteral("にゅ")},
                Symbol{"nyo", QStringLiteral("にょ")},

                Symbol{"hya", QStringLiteral("ひゃ")},
                Symbol{"hyu", QStringLiteral("ひゅ")},
                Symbol{"hyo", QStringLiteral("ひょ")},

                Symbol{"mya", QStringLiteral("みゃ")},
                Symbol{"myu", QStringLiteral("みゅ")},
                Symbol{"myo", QStringLiteral("みょ")},

                Symbol{"rya", QStringLiteral("りゃ")},
                Symbol{"ryu", QStringLiteral("りゅ")},
                Symbol{"ryo", QStringLiteral("りょ")}
            },
            3
        ),

        SymbolsTableSection
        (
            "Yōon with (han)dakuten",
            std::vector<Symbol>
            {
                Symbol{"gya", QStringLiteral("ぎゃ")},
                Symbol{"gyu", QStringLiteral("ぎゅ")},
                Symbol{"gyo", QStringLiteral("ぎょ")},

                Symbol{"zya/ja", QStringLiteral("じゃ")},
                Symbol{"zyu/ju", QStringLiteral("じゅ")},
                Symbol{"zyo/jo", QStringLiteral("じょ")},

                Symbol{"dya/ja", QStringLiteral("ぢゃ")},
                Symbol{"dyu/ju", QStringLiteral("ぢゅ")},
                Symbol{"dyo/jo", QStringLiteral("ぢょ")},

                Symbol{"bya", QStringLiteral("びゃ")},
                Symbol{"byu", QStringLiteral("びゅ")},
                Symbol{"byo", QStringLiteral("びょ")},

                Symbol{"pya", QStringLiteral("ぴゅ")},
                Symbol{"pyu", QStringLiteral("ぴゅ")},
                Symbol{"pyo", QStringLiteral("ぴょ")}
            },
            3
        )
    }
);

//========================================== KATAKANA ==========================================
SymbolsTableFamily SymbolsTables::KatakanaSymbolsTableFamily
(
    "Katakana",
    std::vector<SymbolsTableSection>
    {
        SymbolsTableSection
        (
            "Gojūon",
            std::vector<Symbol>
            {
                Symbol{"a", QStringLiteral("ア")},
                Symbol{"i", QStringLiteral("イ")},
                Symbol{"u", QStringLiteral("ウ")},
                Symbol{"e", QStringLiteral("エ")},
                Symbol{"o", QStringLiteral("オ")},

                Symbol{"ka", QStringLiteral("カ")},
                Symbol{"ki", QStringLiteral("キ")},
                Symbol{"ku", QStringLiteral("ク")},
                Symbol{"ke", QStringLiteral("ケ")},
                Symbol{"ko", QStringLiteral("コ")},


                Symbol{"sa", QStringLiteral("サ")},
                Symbol{"shi", QStringLiteral("シ")},
                Symbol{"su", QStringLiteral("ス")},
                Symbol{"se", QStringLiteral("セ")},
                Symbol{"so", QStringLiteral("ソ")},

                Symbol{"ta", QStringLiteral("タ")},
                Symbol{"chi", QStringLiteral("チ")},
                Symbol{"tsu", QStringLiteral("ツ")},
                Symbol{"te", QStringLiteral("テ")},
                Symbol{"to", QStringLiteral("ト")},

                Symbol{"na", QStringLiteral("ナ")},
                Symbol{"ni", QStringLiteral("ニ")},
                Symbol{"nu", QStringLiteral("ヌ")},
                Symbol{"ne", QStringLiteral("ネ")},
                Symbol{"no", QStringLiteral("ノ")},

                Symbol{"ha", QStringLiteral("ハ")},
                Symbol{"hi", QStringLiteral("ヒ")},
                Symbol{"fu", QStringLiteral("フ")},
                Symbol{"he", QStringLiteral("ヘ")},
                Symbol{"ho", QStringLiteral("ホ")},

                Symbol{"ma", QStringLiteral("マ")},
                Symbol{"mi", QStringLiteral("ミ")},
                Symbol{"mu", QStringLiteral("ム")},
                Symbol{"me", QStringLiteral("メ")},
                Symbol{"mo", QStringLiteral("モ")},

                Symbol{"ya", QStringLiteral("ヤ")},
                Symbol{"yu", QStringLiteral("ユ")},
                Symbol{"yo", QStringLiteral("ヨ")},

                Symbol{"ra", QStringLiteral("ラ")},
                Symbol{"ri", QStringLiteral("リ")},
                Symbol{"ru", QStringLiteral("ル")},
                Symbol{"re", QStringLiteral("レ")},
                Symbol{"ro", QStringLiteral("ロ")},

                Symbol{"wa", QStringLiteral("ワ")},
                Symbol{"(w)o", QStringLiteral("ヲ")},
                Symbol{"n", QStringLiteral("ン")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Gojūon with (han)dakuten",
            std::vector<Symbol>
            {
                Symbol{"ga", QStringLiteral("ガ")},
                Symbol{"gi", QStringLiteral("ギ")},
                Symbol{"gu", QStringLiteral("グ")},
                Symbol{"ge", QStringLiteral("ゲ")},
                Symbol{"go", QStringLiteral("ゴ")},

                Symbol{"za", QStringLiteral("ザ")},
                Symbol{"zi", QStringLiteral("ジ")},
                Symbol{"zu", QStringLiteral("ズ")},
                Symbol{"ze", QStringLiteral("ゼ")},
                Symbol{"zo", QStringLiteral("ゾ")},

                Symbol{"da", QStringLiteral("ダ")},
                Symbol{"ji", QStringLiteral("ヂ")},
                Symbol{"(d)zu", QStringLiteral("ヅ")},
                Symbol{"de", QStringLiteral("デ")},
                Symbol{"do", QStringLiteral("ド")},

                Symbol{"ba", QStringLiteral("バ")},
                Symbol{"bi", QStringLiteral("ビ")},
                Symbol{"bu", QStringLiteral("ブ")},
                Symbol{"be", QStringLiteral("ベ")},
                Symbol{"bo", QStringLiteral("ボ")},

                Symbol{"pa", QStringLiteral("パ")},
                Symbol{"pi", QStringLiteral("ピ")},
                Symbol{"pu", QStringLiteral("プ")},
                Symbol{"pe", QStringLiteral("ペ")},
                Symbol{"po", QStringLiteral("ポ")}
            },
            5
        ),

        SymbolsTableSection
        (
            "Yōon",
            std::vector<Symbol>
            {
                Symbol{"kya", QStringLiteral("キャ")},
                Symbol{"kyu", QStringLiteral("キュ")},
                Symbol{"kyo", QStringLiteral("キョ")},

                Symbol{"sha", QStringLiteral("シャ")},
                Symbol{"shu", QStringLiteral("シュ")},
                Symbol{"sho", QStringLiteral("ショ")},

                Symbol{"cha", QStringLiteral("チャ")},
                Symbol{"chu", QStringLiteral("チュ")},
                Symbol{"cho", QStringLiteral("チョ")},

                Symbol{"nya", QStringLiteral("ニャ")},
                Symbol{"nyu", QStringLiteral("ニュ")},
                Symbol{"nyo", QStringLiteral("ニョ")},

                Symbol{"hya", QStringLiteral("ヒャ")},
                Symbol{"hyu", QStringLiteral("ヒュ")},
                Symbol{"hyo", QStringLiteral("ヒョ")},

                Symbol{"mya", QStringLiteral("ミャ")},
                Symbol{"myu", QStringLiteral("ミュ")},
                Symbol{"myo", QStringLiteral("ミョ")},

                Symbol{"rya", QStringLiteral("リャ")},
                Symbol{"ryu", QStringLiteral("リュ")},
                Symbol{"ryo", QStringLiteral("リョ")},
            },
            3
        ),

        SymbolsTableSection
        (
            "Yōon with (han)dakuten",
            std::vector<Symbol>
            {
                Symbol{"gya", QStringLiteral("ギャ")},
                Symbol{"gyu", QStringLiteral("ギュ")},
                Symbol{"gyo", QStringLiteral("ギョ")},

                Symbol{"zya/ja", QStringLiteral("ジャ")},
                Symbol{"zyu/ju", QStringLiteral("ジュ")},
                Symbol{"zyo/jo", QStringLiteral("ジョ")},

                Symbol{"dya/ja", QStringLiteral("ヂャ")},
                Symbol{"dyu/ju", QStringLiteral("ヂュ")},
                Symbol{"dyo/jo", QStringLiteral("ヂョ")},

                Symbol{"bya", QStringLiteral("ビャ")},
                Symbol{"byu", QStringLiteral("ビュ")},
                Symbol{"byo", QStringLiteral("ビョ")},

                Symbol{"pya", QStringLiteral("ピャ")},
                Symbol{"pyu", QStringLiteral("ピュ")},
                Symbol{"pyo", QStringLiteral("ピョ")}
            },
            3
        )
    }
);
