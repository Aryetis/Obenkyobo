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

    QFont& GetCurrentHiraganaFnt() { return hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)]; }
    QFont& GetCurrentKatakanaFnt() { return katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)]; }
    QFont& GetCurrentRomanjiFnt() { return romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)]; }

    int GetStemBoostSize() const { return stemBoostSize; }

private slots:
    void on_hiraganaDropdown_activated(int index);
    void on_KatakanaDropdown_activated(int index);
    void on_RomanjiDropdown_activated(int index);

    void on_HiraganaSizeSlider_valueChanged(int size);
    void on_KatakanaSizeSlider_valueChanged(int size);
    void on_RomanjiSizeSlider_valueChanged(int size);

    void on_BoostStemSlider_valueChanged(int size);

private:
    Ui::FntSetting *ui;

    void RegisterFntsFromResources();

    void RegisterHiraganaFont(QString fntAddress);
    void RegisterKatakanaFont(QString fntAddress);
    void RegisterRomanjiFont(QString fntAddress);

    QFont GetFont(QString fntAddress);

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
};

#endif // FNTSETTING_H
