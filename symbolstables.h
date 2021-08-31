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
    Symbol(std::string r_, QString j_)
        :romanji(r_), jp(j_)
    {}

    std::string romanji;
    QString jp;
};

namespace SymbolsTables
{
    static const std::vector<Symbol> HIRAGANA_GOJUON =
    {
        Symbol{"a", QStringLiteral("あ")},
        Symbol{"i", QStringLiteral("い")},
        Symbol{"u", QStringLiteral("う")},
        Symbol{"e", QStringLiteral("え")},
        Symbol{"o", QStringLiteral("お")},
        Symbol{"ka", QStringLiteral( u"か")},
        Symbol{"ki", QStringLiteral( u"き")},
        Symbol{"ku", QStringLiteral( u"く")},
        Symbol{"ke", QStringLiteral( u"け")},
        Symbol{"ko", QStringLiteral( u"こ")}
    };

//    static const std::vector<std::string> HIRAGANA_GOJUON_HANDAKUTEN =
//    {"ga","gi","gu","ge","go"};

//    static const std::vector<std::string> HIRAGANA_YOON =
//    {"kya","kyu"};

//    static const std::vector<std::string> HIRAGANA_YOON_HANDAKUTEN =
//    {"gya","gyu"};
}

#endif // SYMBOLSTABLES_H
