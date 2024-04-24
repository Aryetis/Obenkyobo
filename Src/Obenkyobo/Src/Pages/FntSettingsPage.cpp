#include <QDirIterator>
#include <QDir>
#include <QFontDatabase>
#include "Src/Pages/FntSettingsPage.h"
#include "ui_FntSettingsPage.h"
#include "Src/GetMy.h"
#include "Src/DefinesLand.h"

FntSettingsPage::FntSettingsPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::FntSettingsPage),
    settingsSerializer(*GetMy::Instance().SettingSerializerInst())
{
    ui->setupUi(this);

    // Fonts must be ready before switching widget => don't delay the initialization
    RegisterFntsFromResources(); // must be done before anything needing fonts => don't move into InitializeFntSetting
    GetMy::Instance().SetFntSettingsPageInst(this);
}

FntSettingsPage::~FntSettingsPage()
{
    delete ui;
}

void FntSettingsPage::SetKanasAnswerRmjKanaSize(int size)
{
    on_KanasAnswerRmjKanaSlider_valueChanged(size);
    ui->KanasAnswerRmjKanaSlider->setValue(size);
}

void FntSettingsPage::SetKanasAnswerKanaRmjSize(int size)
{
    on_KanasAnswerKanaRmjSlider_valueChanged(size);
    ui->KanasAnswerKanaRmjSlider->setValue(size);
}

void FntSettingsPage::SetKanasStemSize(int size)
{
    on_KanasStemSlider_valueChanged(size);
    ui->KanasStemSlider->setValue(size);
}

void FntSettingsPage::SetVocabAnswerRmjKanaSize(int size)
{
    on_VocabAnswerRmjKanaSlider_valueChanged(size);
    ui->VocabAnswerRmjKanaSlider->setValue(size);
}

void FntSettingsPage::SetVocabAnswerKanaRmjSize(int size)
{
    on_VocabAnswerKanaRmjSlider_valueChanged(size);
    ui->VocabAnswerKanaRmjSlider->setValue(size);
}

void FntSettingsPage::SetVocabStemSize(int size)
{
    on_VocabStemSlider_valueChanged(size);
    ui->VocabStemSlider->setValue(size);
}

void FntSettingsPage::RegisterFntsFromResources()
{
    std::unique_ptr<QDirIterator> it (new QDirIterator(":/HiraganaFonts",  {"*.ttf"}, QDir::Files));
    while (it->hasNext())
        RegisterHiraganaFont(it->next());

    it.reset(new QDirIterator(":/KatakanaFonts",  {"*.ttf"}, QDir::Files));
    while (it->hasNext())
        RegisterKatakanaFont(it->next());

    it.reset(new QDirIterator(":/RomanjiFonts",  {"*.ttf"}, QDir::Files));
    while (it->hasNext())
        RegisterRomanjiFont(it->next());

    it.reset(new QDirIterator(":/VersatileFonts",  {"*.ttf"}, QDir::Files));
    while (it->hasNext())
    {
        QString fntAddress = it->next();
        RegisterHiraganaFont(fntAddress);
        RegisterKatakanaFont(fntAddress);
        RegisterRomanjiFont(fntAddress);
    }

    it.reset (new QDirIterator(":/KanjiFonts",  {"*.otf"}, QDir::Files));
    while (it->hasNext())
        RegisterKanjiFont(it->next());

    for (QFont const& fnt : hiraganaFonts)
        ui->HiraganaFntDropdown->addItem(fnt.family());
    for (QFont const& fnt : katakanaFonts)
        ui->KatakanaFntDropdown->addItem(fnt.family());
    for (QFont const& fnt : romanjiFonts)
        ui->RomanjiFntDropdown->addItem(fnt.family());
    for (QFont const& fnt : kanjiFonts)
        ui->KanjiFntDropdown->addItem(fnt.family());

//    Works just fine but I can't stand Roboto Mono... Plus it's way larger => bad for small screens
//    QFont globalFont("Roboto Mono");
//    globalFont.setStyleHint(QFont::Monospace);
//    QApplication::setFont(globalFont);

    currentHiraganFntIdx = settingsSerializer.value("FntSettings/HiraganaFntIdx", 0).toInt();
    currentKatakanaFntIdx = settingsSerializer.value("FntSettings/KatakanaFntIdx", 0).toInt();
    currentRomanjiFntIdx = settingsSerializer.value("FntSettings/RomanjiFntIdx", 0).toInt();
    currentKanjiFontIdx = settingsSerializer.value("FntSettings/KanjiFntIdx", 0).toInt();
    ui->HiraganaFntDropdown->setCurrentIndex(currentHiraganFntIdx);
    ui->KatakanaFntDropdown->setCurrentIndex(currentKatakanaFntIdx);
    ui->RomanjiFntDropdown->setCurrentIndex(currentRomanjiFntIdx);
    ui->KanjiFntDropdown->setCurrentIndex(currentKanjiFontIdx);

    kanasStemSize =  settingsSerializer.value("FntSettings/KanasStemSize", DEFAULT_KANAS_STEM_FNT_SIZE).toInt();
    ui->KanasStemSlider->setValue(kanasStemSize);
    ui->KanasStemValueLabel->setText(QString::number(kanasStemSize));
    kanasAnswerRmjKanaSize =  settingsSerializer.value("FntSettings/answerRmjKanaSize", DEFAULT_KANAS_ANSWER_RMJ_KANA_FNT_SIZE).toInt();
    ui->KanasAnswerRmjKanaSlider->setValue(kanasAnswerRmjKanaSize);
    ui->KanasAnswerRmjKanaValueLabel->setText(QString::number(kanasAnswerRmjKanaSize));
    kanasAnswerKanaRmjSize =  settingsSerializer.value("FntSettings/answerKanaRmjSize", DEFAULT_KANAS_ANSWER_KANA_RMJ_SIZE).toInt();
    ui->KanasAnswerKanaRmjSlider->setValue(kanasAnswerKanaRmjSize);
    ui->KanasAnswerKanaRmjValueLabel->setText(QString::number(kanasAnswerKanaRmjSize));

    vocabStemSize =  settingsSerializer.value("FntSettings/VocabStemSize", DEFAULT_VOCAB_STEM_FNT_SIZE).toInt();
    ui->VocabStemSlider->setValue(vocabStemSize);
    ui->VocabStemValueLabel->setText(QString::number(vocabStemSize));
    vocabAnswerRmjKanaSize =  settingsSerializer.value("FntSettings/answerRmjVocabSize", DEFAULT_VOCAB_ANSWER_RMJ_KANA_FNT_SIZE).toInt();
    ui->VocabAnswerRmjKanaSlider->setValue(vocabAnswerRmjKanaSize);
    ui->VocabAnswerRmjKanaValueLabel->setText(QString::number(vocabAnswerRmjKanaSize));
    vocabAnswerKanaRmjSize =  settingsSerializer.value("FntSettings/answerVocabRmjSize", DEFAULT_VOCAB_ANSWER_KANA_RMJ_SIZE).toInt();
    ui->VocabAnswerKanaRmjSlider->setValue(vocabAnswerKanaRmjSize);
    ui->VocabAnswerKanaRmjValueLabel->setText(QString::number(vocabAnswerKanaRmjSize));
}

void FntSettingsPage::RegisterHiraganaFont(QString fntAddress)
{
    hiraganaFonts.emplace_back(GetFont(fntAddress));
}

void FntSettingsPage::RegisterKatakanaFont(QString fntAddress)
{
    katakanaFonts.emplace_back(GetFont(fntAddress));
}

void FntSettingsPage::RegisterRomanjiFont(QString fntAddress)
{
    romanjiFonts.emplace_back(GetFont(fntAddress));
}

void FntSettingsPage::RegisterKanjiFont(QString fntAddress)
{
    kanjiFonts.emplace_back(GetFont(fntAddress));
}

QFont FntSettingsPage::GetFont(QString fntAddress)
{
    int id = QFontDatabase::addApplicationFont(fntAddress);
    QString name = QFontDatabase::applicationFontFamilies(id).at(0);

    return QFont(name);
}

//===================================================================================

void FntSettingsPage::on_HiraganaFntDropdown_activated(int index)
{
    currentHiraganFntIdx = index;
    settingsSerializer.setValue("FntSettings/HiraganaFntIdx", index);
    hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)].setFamily(
                ui->HiraganaFntDropdown->itemText(currentHiraganFntIdx));
}

void FntSettingsPage::on_KatakanaFntDropdown_activated(int index)
{
    currentKatakanaFntIdx = index;
    settingsSerializer.setValue("FntSettings/KatakanaFntIdx", index);
    katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)].setFamily(
                ui->KatakanaFntDropdown->itemText(currentKatakanaFntIdx));
}

void FntSettingsPage::on_RomanjiFntDropdown_activated(int index)
{
    currentRomanjiFntIdx = index;
    settingsSerializer.setValue("FntSettings/RomanjiFntIdx", index);
    romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)].setFamily(
                ui->RomanjiFntDropdown->itemText(currentRomanjiFntIdx));
}

void FntSettingsPage::on_KanjiFntDropdown_currentIndexChanged(int index)
{
    currentKanjiFontIdx = index;
    settingsSerializer.setValue("FntSettings/KanjiFntIdx", index);
    romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentKanjiFontIdx)].setFamily(
                ui->RomanjiFntDropdown->itemText(currentKanjiFontIdx));
}

//===================================================================================

void FntSettingsPage::on_KanasStemSlider_valueChanged(int size)
{
    kanasStemSize = size;
    ui->KanasStemValueLabel->setText(QString::number(size));
    settingsSerializer.setValue("FntSettings/KanasStemSize", size);
}

void FntSettingsPage::on_KanasAnswerKanaRmjSlider_valueChanged(int size)
{
    kanasAnswerKanaRmjSize = size;
    ui->KanasAnswerKanaRmjValueLabel->setText(QString::number(size));
    settingsSerializer.setValue("FntSettings/answerKanaRmjSize", size);
}

void FntSettingsPage::on_KanasAnswerRmjKanaSlider_valueChanged(int size)
{
    kanasAnswerRmjKanaSize = size;
    ui->KanasAnswerRmjKanaValueLabel->setText(QString::number(size));
    settingsSerializer.setValue("FntSettings/answerRmjKanaSize", size);
}

//===================================================================================


void FntSettingsPage::on_VocabStemSlider_valueChanged(int size)
{
    vocabStemSize = size;
    ui->VocabStemValueLabel->setText(QString::number(size));
    settingsSerializer.setValue("FntSettings/VocabStemSize", size);
}

void FntSettingsPage::on_VocabAnswerKanaRmjSlider_valueChanged(int size)
{
    vocabAnswerKanaRmjSize = size;
    ui->VocabAnswerKanaRmjValueLabel->setText(QString::number(size));
    settingsSerializer.setValue("FntSettings/answerVocabRmjSize", size);
}

void FntSettingsPage::on_VocabAnswerRmjKanaSlider_valueChanged(int size)
{
    vocabAnswerRmjKanaSize = size;
    ui->VocabAnswerRmjKanaValueLabel->setText(QString::number(size));
    settingsSerializer.setValue("FntSettings/answerRmjVocabSize", size);
}
