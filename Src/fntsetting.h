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
    QFont const& GetCurrentKanjiFnt() { return kanjiFonts[static_cast<std::vector<QFont>::size_type>(currentKanjiFontIdx)]; }

    QString GetCurrentHiraganaFamily() { return hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)].family(); }
    QString GetCurrentKatakanaFamily() { return katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)].family(); }
    QString GetCurrentRomanjiFamily() { return romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)].family(); }
    QString GetCurrentKanjiFamily() { return kanjiFonts[static_cast<std::vector<QFont>::size_type>(currentKanjiFontIdx)].family(); }

    int GetStemSize() const { return stemSize; }
    int GetAnswerRmjKanaSize() const { return answerRmjKanaSize; }
    int GetAnswerKanaRmjSize() const { return answerKanaRmjSize; }
    void SetAnswerRmjKanaSize(int size);
    void SetAnswerKanaRmjSize(int size);

private slots:
    void on_HiraganaFntDropdown_activated(int index);
    void on_KatakanaFntDropdown_activated(int index);
    void on_RomanjiFntDropdown_activated(int index);

    void on_KanjiFntDropdown_currentIndexChanged(int index);

    void on_StemSlider_valueChanged(int size);
    void on_AnswerRmjKanaSlider_valueChanged(int size);
    void on_AnswerKanaRmjSlider_valueChanged(int size);

private:
    Ui::FntSetting *ui;

    void RegisterFntsFromResources();

    void RegisterHiraganaFont(QString fntAddress);
    void RegisterKatakanaFont(QString fntAddress);
    void RegisterRomanjiFont(QString fntAddress);
    void RegisterKanjiFont(QString fntAddress);

    QFont GetFont(QString fntAddress);

    std::vector<QFont> hiraganaFonts{};
    std::vector<QFont> katakanaFonts{};
    std::vector<QFont> romanjiFonts{};
    std::vector<QFont> kanjiFonts{};
    int currentHiraganFntIdx;
    int currentKatakanaFntIdx;
    int currentRomanjiFntIdx;
    int currentKanjiFontIdx;

    int stemSize;
    int answerRmjKanaSize;
    int answerKanaRmjSize;

    QSettings* settingsSerializer;
};

#endif // FNTSETTING_H
