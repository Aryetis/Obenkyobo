#ifndef FNTSETTING_H
#define FNTSETTING_H
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
