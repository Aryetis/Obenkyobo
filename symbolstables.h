#ifndef SYMBOLSTABLES_H
#define SYMBOLSTABLES_H
//#include <map>
#include <vector>
#include <string>

// <romanji, symbol>
// TODO create proper class later on so we can only initalize exercice with GOJUON/GOJUON_HANDAKUTEN/etc string
// handle disable / enable set => handle edge cases like "not enough data to initialize entriesGridLayout", etc
namespace SymbolsTables
{
    static const std::vector<std::string> HIRAGANA_GOJUON =
    {"a","i","u","e","o","ka","ki","ku","ke","ko"};

    static const std::vector<std::string> HIRAGANA_GOJUON_HANDAKUTEN =
    {"ga","gi","gu","ge","go"};

    static const std::vector<std::string> HIRAGANA_YOON =
    {"kya","kyu"};

    static const std::vector<std::string> HIRAGANA_YOON_HANDAKUTEN =
    {"gya","gyu"};
}

//static const std::map<std::string, std::string> HIRAGANA_GOJUON =
//{
//    {"a","a"},
//    {"i","a"},
//    {"u","u"},
//    {"e","e"},
//    {"o","o"},
//    {"ka","ka"},
//    {"ki","ki"},
//    {"ku","ku"},
//    {"ke","ke"},
//    {"ko","ko"}
//};

//static const std::map<std::string, std::string> HIRAGANA_GOJUON_HANDAKUTEN =
//{
//    {"ga","ga"},
//    {"gi","ga"},
//    {"gu","gu"},
//    {"ge","ge"},
//    {"go","go"},
//};

//static const std::map<std::string, std::string> HIRAGANA_YOON =
//{
//    {"kya","kya"},
//    {"kyu","kyu"}
//};

//static const std::map<std::string, std::string> HIRAGANA_YOON_HANDAKUTEN =
//{
//    {"gya","gya"},
//    {"gyu","gyu"}
//};

#endif // SYMBOLSTABLES_H
