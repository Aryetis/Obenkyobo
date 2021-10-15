#ifndef FNTSETTING_H
#define FNTSETTING_H
#include <QWidget>
#include <QString>
#include <vector>
#include <QSettings>

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

    QFont const& GetCurrentHiraganaFnt() { return hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)]; }
    QFont const& GetCurrentKatakanaFnt() { return katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)]; }
    QFont const& GetCurrentRomanjiFnt() { return romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)]; }

    QString GetCurrentHiraganaFamily() { return hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)].family(); }
    QString GetCurrentKatakanaFamily() { return katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)].family(); }
    QString GetCurrentRomanjiFamily() { return romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)].family(); }

    int GetCurrentHiraganaSize() { return currentHiraganaSize; }
    int GetCurrentKatakanaSize() { return currentKatakanaSize; }
    int GetCurrentRomanjiSize() { return currentRomanjiSize; }

    int GetStemBoostSize() const { return stemBoostSize; }

private slots:
    void on_HiraganaFntDropdown_activated(int index);
    void on_KatakanaFntDropdown_activated(int index);
    void on_RomanjiFntDropdown_activated(int index);

    void on_HiraganaSizeSlider_valueChanged(int size);
    void on_KatakanaSizeSlider_valueChanged(int size);
    void on_RomanjiSizeSlider_valueChanged(int size);

    void on_BoostStemSlider_valueChanged(int size);

private:
    enum fntTypeEnum
    {
        hiragana,
        katakana,
        romanji
    };
    Ui::FntSetting *ui;

    void RegisterFntsFromResources();

    void RegisterHiraganaFont(QString fntAddress);
    void RegisterKatakanaFont(QString fntAddress);
    void RegisterRomanjiFont(QString fntAddress);

    QFont GetFont(QString fntAddress, fntTypeEnum type);

    std::vector<QFont> hiraganaFonts{};
    std::vector<QFont> katakanaFonts{};
    std::vector<QFont> romanjiFonts{};
    int currentHiraganFntIdx;
    int currentKatakanaFntIdx;
    int currentRomanjiFntIdx;
    int currentHiraganaSize;
    int currentKatakanaSize;
    int currentRomanjiSize;

    int stemBoostSize;

    QSettings* settingsSerializer;
};

#endif // FNTSETTING_H
