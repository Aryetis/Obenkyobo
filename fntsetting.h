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

    QFont& GetCurrentHiraganaFnt() { return currentHiraganaFnt; }
    QFont& GetCurrentKatakanaFnt() { return currentKatakanaFnt; }
    QFont& GetCurrentRomanjiFnt() { return currentRomanjiFnt; }

private slots:
    void on_hiraganaDropdown_activated(const QString &arg1);
    void on_KatakanaDropdown_activated(const QString &arg1);
    void on_RomanjiDropdown_activated(const QString &arg1);

private:
    void RegisterFntsFromResources();

    void RegisterHiraganaFont(QString fntAddress);
    void RegisterKatakanaFont(QString fntAddress);
    void RegisterRomanjiFont(QString fntAddress);

    Ui::FntSetting *ui;

    QString GetFontName(QString fntAddress);
    std::vector<QString> hiraganaFontsNames{};
    std::vector<QString> katakanaFontsNames{};
    std::vector<QString> romanjiFontsNames{};
    QFont currentHiraganaFnt;
    QFont currentKatakanaFnt;
    QFont currentRomanjiFnt;
};

#endif // FNTSETTING_H
