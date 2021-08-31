#ifndef SYMBOLSTABLES_H
#define SYMBOLSTABLES_H
#include <vector>
#include <string>
#include <QStringLiteral>
#include <QString>

// <romanji, symbol>
// TODO create proper class later on so we can only initalize exercice with GOJUON/GOJUON_HANDAKUTEN/etc string
// handle disable / enable set => handle edge cases like "not enough data to initialize entriesGridLayout", etc
struct Symbol
{
    Symbol() = default;
    Symbol(std::string r_, QString j_)
        :romanji(r_), jp(j_)
    {}

    bool operator==(Symbol const & rhs) const
    {
        return (this->romanji== rhs.romanji && this->jp == rhs.jp);
    }
    std::string romanji;
    QString jp;
};

namespace SymbolsTables
{
    //========================================== HIRAGANA ==========================================
    static const std::vector<Symbol> HIRAGANA_GOJUON =
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
        Symbol{"si", QStringLiteral("し")},
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
        Symbol{"n", QStringLiteral("ん")},
    };

    static const std::vector<Symbol> HIRAGANA_GOJUON_HANDAKUTEN =
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
        Symbol{"ji", QStringLiteral("じ")},
        Symbol{"zu", QStringLiteral("ず")},
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
        Symbol{"po", QStringLiteral("ぽ")},
    };

    static const std::vector<Symbol> HIRAGANA_YOON =
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
        Symbol{"ryo", QStringLiteral("りょ")},
    };


    static const std::vector<Symbol> HIRAGANA_YOON_HANDAKUTEN =
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

        Symbol{"bya", QStringLiteral("ぎゃ")},
        Symbol{"byu", QStringLiteral("ぎゅ")},
        Symbol{"byo", QStringLiteral("ぎょ")},

        Symbol{"pya", QStringLiteral("きゅ")},
        Symbol{"pyu", QStringLiteral("きゅ")},
        Symbol{"pyo", QStringLiteral("きょ")},
    };

    //========================================== KATAKANA ==========================================
}

#endif // SYMBOLSTABLES_H
