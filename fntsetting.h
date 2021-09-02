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

class FntSetting final : public QWidget
{
    Q_OBJECT

public:
    ~FntSetting() override;
    static FntSetting& GetInstance()
    {
        static FntSetting instance;
        return instance;
    }

    void InitializeFntSetting();

    void RegisterHiraganaFont(std::string fntAddress);
    void RegisterKatakanaFont(std::string fntAddress);
    void RegisterRomanjiFont(std::string fntAddress);
    QFont& GetCurrentHiraganaFnt() { return currentHiraganaFnt; }
    QFont& GetCurrentKatakanaFnt() { return currentKatakanaFnt; }
    QFont& GetCurrentRomanjiFnt() { return currentRomanjiFnt; }

private slots:
    void on_hiraganaDropdown_activated(const QString &arg1);
    void on_KatakanaDropdown_activated(const QString &arg1);
    void on_RomanjiDropdown_activated(const QString &arg1);

private:
    explicit FntSetting(QWidget *parent = nullptr);
    Ui::FntSetting *ui;

    QString GetFontName(std::string fntAddress);
    std::vector<QString> hiraganaFonts{};
    std::vector<QString> katakanaFonts{};
    std::vector<QString> romanjiFonts{};
    QFont currentHiraganaFnt;
    QFont currentKatakanaFnt;
    QFont currentRomanjiFnt;
};

#endif // FNTSETTING_H
