#include "fntsetting.h"
#include "ui_fntsetting.h"
#include <QFontDatabase>
#include "GetMy.h"
#include <QDirIterator>
#include <QDir>
#include "SettingsSerializer.h"

#define DEFAULT_FNT_SIZE 50
#define DEFAULT_STEM_BOOST_SIZE 20

// TODO set max size of fonts depending of screen size and dpi
// TODO add an option to make stem [0;30] biger than entries

FntSetting::FntSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FntSetting)
{
    ui->setupUi(this);

    RegisterFntsFromResources();

    GetMy::GetInstance().SetFntSettingWidget(this);
}

FntSetting::~FntSetting()
{
    delete ui;
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

    for (QFont fnt : hiraganaFonts)
        ui->HiraganaFntDropdown->addItem(fnt.family());
    for (QFont fnt : katakanaFonts)
        ui->KatakanaFntDropdown->addItem(fnt.family());
    for (QFont fnt : romanjiFonts)
        ui->RomanjiFntDropdown->addItem(fnt.family());

    currentHiraganFntIdx = SettingsSerializer::settings.value("FntSettings/HiraganaFntIdx", 0).toInt();
    currentKatakanaFntIdx = SettingsSerializer::settings.value("FntSettings/KatakanaFntIdx", 0).toInt();
    currentRomanjiFntIdx = SettingsSerializer::settings.value("FntSettings/RomanjiFntIdx", 0).toInt();
    ui->HiraganaFntDropdown->setCurrentIndex(currentHiraganFntIdx);
    ui->KatakanaFntDropdown->setCurrentIndex(currentKatakanaFntIdx);
    ui->RomanjiFntDropdown->setCurrentIndex(currentRomanjiFntIdx);

    currentHiraganaSize =  SettingsSerializer::settings.value("FntSettings/HiraganaFntSize", DEFAULT_FNT_SIZE).toInt();
    currentKatakanaSize =  SettingsSerializer::settings.value("FntSettings/KatakanaFntSize", DEFAULT_FNT_SIZE).toInt();
    currentRomanjiSize =  SettingsSerializer::settings.value("FntSettings/RomanjiFntSize", DEFAULT_FNT_SIZE).toInt();
    ui->HiraganaSizeValueLabel->setText(QString::number(currentHiraganaSize));
    ui->HiraganaSizeSlider->setValue(currentHiraganaSize);
    ui->KatakanaSizeValueLabel->setText(QString::number(currentKatakanaSize));
    ui->KatakanaSizeSlider->setValue(currentKatakanaSize);
    ui->RomanjiSizeValueLabel->setText(QString::number(currentRomanjiSize));
    ui->RomanjiSizeSlider->setValue(currentRomanjiSize);

    stemBoostSize =  SettingsSerializer::settings.value("FntSettings/stemBoostSize", DEFAULT_STEM_BOOST_SIZE).toInt();
    ui->BoostStemSlider->setValue(stemBoostSize);
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

QFont FntSetting::GetFont(QString fntAddress)
{
    int id = QFontDatabase::addApplicationFont(fntAddress);
    QString name = QFontDatabase::applicationFontFamilies(id).at(0);
    return QFont(name, DEFAULT_FNT_SIZE);
}

void FntSetting::on_HiraganaFntDropdown_activated(int index)
{
    currentHiraganFntIdx = index;
    SettingsSerializer::settings.setValue("FntSettings/HiraganaFntIdx", index);
    hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)].setFamily(
                ui->HiraganaFntDropdown->itemText(currentHiraganFntIdx));
}

void FntSetting::on_KatakanaFntDropdown_activated(int index)
{
    currentKatakanaFntIdx = index;
    SettingsSerializer::settings.setValue("FntSettings/KatakanaFntIdx", index);
    katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)].setFamily(
                ui->KatakanaFntDropdown->itemText(currentKatakanaFntIdx));
}

void FntSetting::on_RomanjiFntDropdown_activated(int index)
{
    currentRomanjiFntIdx = index;
    SettingsSerializer::settings.setValue("FntSettings/RomanjiFntIdx", index);
    romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)].setFamily(
                ui->RomanjiFntDropdown->itemText(currentRomanjiFntIdx));
}

void FntSetting::on_HiraganaSizeSlider_valueChanged(int size)
{
    currentHiraganaSize = size;
    ui->HiraganaSizeValueLabel->setText(QString::number(size));
    SettingsSerializer::settings.setValue("FntSettings/HiraganaFntSize", size);
    hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)].setPixelSize(size);
}

void FntSetting::on_KatakanaSizeSlider_valueChanged(int size)
{
    currentKatakanaSize = size;
    ui->KatakanaSizeValueLabel->setText(QString::number(size));
    SettingsSerializer::settings.setValue("FntSettings/KatakanaFntSize", size);
    katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)].setPixelSize(size);
}

void FntSetting::on_RomanjiSizeSlider_valueChanged(int size)
{
    currentRomanjiSize = size;
    ui->RomanjiSizeValueLabel->setText(QString::number(size));
    SettingsSerializer::settings.setValue("FntSettings/RomanjiFntSize", size);
    romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)].setPixelSize(size);
}

void FntSetting::on_BoostStemSlider_valueChanged(int size)
{
    stemBoostSize = size;
    ui->BoostStemValueLabel->setText(QString::number(size));
    SettingsSerializer::settings.setValue("FntSettings/stemBoostSize", size);
}
