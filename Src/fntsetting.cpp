#include "Src/fntsetting.h"
#include "ui_fntsetting.h"
#include <QFontDatabase>
#include "Src/GetMy.h"
#include <QDirIterator>
#include <QDir>

#define DEFAULT_ANSWER_RMJ_KANA_FNT_SIZE 25
#define DEFAULT_ANSWER_KANA_RMJ_SIZE 50
#define DEFAULT_STEM_FNT_SIZE 70

FntSetting::FntSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FntSetting),
    settingsSerializer(GetMy::Instance().SettingSerializer())
{
    ui->setupUi(this);

    // Fonts must be ready before switching widget => don't delay the initialization
    RegisterFntsFromResources(); // must be done before anything needing fonts => don't move into InitializeFntSetting
    GetMy::Instance().SetFntSettingWidget(this);
}

FntSetting::~FntSetting()
{
    delete ui;
}

void FntSetting::SetAnswerRmjKanaSize(int size)
{
    on_AnswerRmjKanaSlider_valueChanged(size);
    ui->AnswerRmjKanaSlider->setValue(size);
}

void FntSetting::SetAnswerKanaRmjSize(int size)
{
    on_AnswerKanaRmjSlider_valueChanged(size);
    ui->AnswerKanaRmjSlider->setValue(size);
}

void FntSetting::RegisterFntsFromResources()
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

    for (QFont fnt : hiraganaFonts)
        ui->HiraganaFntDropdown->addItem(fnt.family());
    for (QFont fnt : katakanaFonts)
        ui->KatakanaFntDropdown->addItem(fnt.family());
    for (QFont fnt : romanjiFonts)
        ui->RomanjiFntDropdown->addItem(fnt.family());
    for (QFont fnt : kanjiFonts)
        ui->KanjiFntDropdown->addItem(fnt.family());

//    Works just fine but I can't stand Roboto Mono... Plus it's way larger => bad for small screens
//    QFont globalFont("Roboto Mono");
//    globalFont.setStyleHint(QFont::Monospace);
//    QApplication::setFont(globalFont);

    currentHiraganFntIdx = settingsSerializer->value("FntSettings/HiraganaFntIdx", 0).toInt();
    currentKatakanaFntIdx = settingsSerializer->value("FntSettings/KatakanaFntIdx", 0).toInt();
    currentRomanjiFntIdx = settingsSerializer->value("FntSettings/RomanjiFntIdx", 0).toInt();
    currentKanjiFontIdx = settingsSerializer->value("FntSettings/KanjiFntIdx", 0).toInt();
    ui->HiraganaFntDropdown->setCurrentIndex(currentHiraganFntIdx);
    ui->KatakanaFntDropdown->setCurrentIndex(currentKatakanaFntIdx);
    ui->RomanjiFntDropdown->setCurrentIndex(currentRomanjiFntIdx);
    ui->KanjiFntDropdown->setCurrentIndex(currentKanjiFontIdx);

    stemSize =  settingsSerializer->value("FntSettings/stemSize", DEFAULT_STEM_FNT_SIZE).toInt();
    ui->StemSlider->setValue(stemSize);
    ui->StemValueLabel->setText(QString::number(stemSize));
    answerRmjKanaSize =  settingsSerializer->value("FntSettings/answerRmjKanaSize", DEFAULT_ANSWER_RMJ_KANA_FNT_SIZE).toInt();
    ui->AnswerRmjKanaSlider->setValue(answerRmjKanaSize);
    ui->AnswerRmjKanaValueLabel->setText(QString::number(answerRmjKanaSize));
    answerKanaRmjSize =  settingsSerializer->value("FntSettings/answerKanaRmjSize", DEFAULT_ANSWER_KANA_RMJ_SIZE).toInt();
    ui->AnswerKanaRmjSlider->setValue(answerKanaRmjSize);
    ui->AnswerKanaRmjValueLabel->setText(QString::number(answerKanaRmjSize));
}

void FntSetting::RegisterHiraganaFont(QString fntAddress)
{
    hiraganaFonts.emplace_back(GetFont(fntAddress));
}

void FntSetting::RegisterKatakanaFont(QString fntAddress)
{
    katakanaFonts.emplace_back(GetFont(fntAddress));
}

void FntSetting::RegisterRomanjiFont(QString fntAddress)
{
    romanjiFonts.emplace_back(GetFont(fntAddress));
}

void FntSetting::RegisterKanjiFont(QString fntAddress)
{
    kanjiFonts.emplace_back(GetFont(fntAddress));
}

QFont FntSetting::GetFont(QString fntAddress)
{
    int id = QFontDatabase::addApplicationFont(fntAddress);
    QString name = QFontDatabase::applicationFontFamilies(id).at(0);

    return QFont(name);
}

void FntSetting::on_HiraganaFntDropdown_activated(int index)
{
    currentHiraganFntIdx = index;
    settingsSerializer->setValue("FntSettings/HiraganaFntIdx", index);
    hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)].setFamily(
                ui->HiraganaFntDropdown->itemText(currentHiraganFntIdx));
}

void FntSetting::on_KatakanaFntDropdown_activated(int index)
{
    currentKatakanaFntIdx = index;
    settingsSerializer->setValue("FntSettings/KatakanaFntIdx", index);
    katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)].setFamily(
                ui->KatakanaFntDropdown->itemText(currentKatakanaFntIdx));
}

void FntSetting::on_RomanjiFntDropdown_activated(int index)
{
    currentRomanjiFntIdx = index;
    settingsSerializer->setValue("FntSettings/RomanjiFntIdx", index);
    romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)].setFamily(
                ui->RomanjiFntDropdown->itemText(currentRomanjiFntIdx));
}

void FntSetting::on_KanjiFntDropdown_currentIndexChanged(int index)
{
    currentKanjiFontIdx = index;
    settingsSerializer->setValue("FntSettings/KanjiFntIdx", index);
    romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentKanjiFontIdx)].setFamily(
                ui->RomanjiFntDropdown->itemText(currentKanjiFontIdx));
}

void FntSetting::on_StemSlider_valueChanged(int size)
{
    stemSize = size;
    ui->StemValueLabel->setText(QString::number(size));
    settingsSerializer->setValue("FntSettings/stemSize", size);
}

void FntSetting::on_AnswerRmjKanaSlider_valueChanged(int size)
{
    answerRmjKanaSize = size;
    ui->AnswerRmjKanaValueLabel->setText(QString::number(size));
    settingsSerializer->setValue("FntSettings/answerRmjKanaSize", size);
}

void FntSetting::on_AnswerKanaRmjSlider_valueChanged(int size)
{
    answerKanaRmjSize = size;
    ui->AnswerKanaRmjValueLabel->setText(QString::number(size));
    settingsSerializer->setValue("FntSettings/answerKanaRmjSize", size);
}
