#ifndef FNTSETTING_H
#define FNTSETTING_H
#include <string>
#include <QWidget>
#include <QString>
#include <vector>

namespace Ui
{
    class FntSetting;
}

class FntSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FntSetting(QWidget *parent = nullptr);
    ~FntSetting() override;

    void InitializeFntSetting();

    static void RegisterHiraganaFont(std::string fntAddress);
    static void RegisterKatakanaFont(std::string fntAddress);
    static void RegisterRomanjiFont(std::string fntAddress);
    static QFont& GetCurrentHiraganaFnt() { return currentHiraganaFnt; }
    static QFont& GetCurrentKatakanaFnt() { return currentKatakanaFnt; }
    static QFont& GetCurrentRomanjiFnt() { return currentRomanjiFnt; }

private slots:
    void on_hiraganaDropdown_activated(const QString &arg1);
    void on_KatakanaDropdown_activated(const QString &arg1);
    void on_RomanjiDropdown_activated(const QString &arg1);

private:
    Ui::FntSetting *ui;

    static QString GetFontName(std::string fntAddress);
    inline static std::vector<QString> hiraganaFonts{};
    inline static std::vector<QString> katakanaFonts{};
    inline static std::vector<QString> romanjiFonts{};
    inline static QFont currentHiraganaFnt;
    inline static QFont currentKatakanaFnt;
    inline static QFont currentRomanjiFnt;
};

#endif // FNTSETTING_H
