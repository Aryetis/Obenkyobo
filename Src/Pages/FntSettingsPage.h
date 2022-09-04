#ifndef FNTSETTINGSPAGE_H
#define FNTSETTINGSPAGE_H

#include <QWidget>
#include <QString>
#include <vector>
#include <QSettings>

namespace Ui
{
    class FntSettingsPage;
}

class FntSettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit FntSettingsPage(QWidget *parent = nullptr);
    ~FntSettingsPage() override;

    // Size not applied => not sure if it's really useful... Might as well only store FntFamily instead of QFont then ? TODO
//    QFont const& GetCurrentHiraganaFnt() { return hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)]; }
//    QFont const& GetCurrentKatakanaFnt() { return katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)]; }
//    QFont const& GetCurrentRomanjiFnt() { return romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)]; }
//    QFont const& GetCurrentKanjiFnt() { return kanjiFonts[static_cast<std::vector<QFont>::size_type>(currentKanjiFontIdx)]; }

    QString GetCurrentHiraganaFamily() { return hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)].family(); }
    QString GetCurrentKatakanaFamily() { return katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)].family(); }
    QString GetCurrentRomanjiFamily() { return romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)].family(); }
    QString GetCurrentKanjiFamily() { return kanjiFonts[static_cast<std::vector<QFont>::size_type>(currentKanjiFontIdx)].family(); }

    int GetKanasStemSize() const { return kanasStemSize; }
    int GetKanasAnswerRmjKanaSize() const { return kanasAnswerRmjKanaSize; }
    int GetKanasAnswerKanaRmjSize() const { return kanasAnswerKanaRmjSize; }
    void SetKanasAnswerRmjKanaSize(int size);
    void SetKanasAnswerKanaRmjSize(int size);
    void SetKanasStemSize(int size);

    int GetVocabStemSize() const { return vocabStemSize; }
    int GetVocabAnswerRmjKanaSize() const { return vocabAnswerRmjKanaSize; }
    int GetVocabAnswerKanaRmjSize() const { return vocabAnswerKanaRmjSize; }
    void SetVocabAnswerRmjKanaSize(int size);
    void SetVocabAnswerKanaRmjSize(int size);
    void SetVocabStemSize(int size);

private slots:
    void on_HiraganaFntDropdown_activated(int index);
    void on_KatakanaFntDropdown_activated(int index);
    void on_RomanjiFntDropdown_activated(int index);
    void on_KanjiFntDropdown_currentIndexChanged(int index);

    void on_KanasStemSlider_valueChanged(int value);
    void on_KanasAnswerKanaRmjSlider_valueChanged(int value);
    void on_KanasAnswerRmjKanaSlider_valueChanged(int value);

    void on_VocabStemSlider_valueChanged(int value);
    void on_VocabAnswerKanaRmjSlider_valueChanged(int value);
    void on_VocabAnswerRmjKanaSlider_valueChanged(int value);

private:
    Ui::FntSettingsPage *ui;

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

    int kanasStemSize;
    int kanasAnswerRmjKanaSize;
    int kanasAnswerKanaRmjSize;

    int vocabStemSize;
    int vocabAnswerRmjKanaSize;
    int vocabAnswerKanaRmjSize;

    QSettings* settingsSerializer;
};

#endif // FNTSETTINGSPAGE_H
