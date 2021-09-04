#include "fntsetting.h"
#include "ui_fntsetting.h"
#include <QFontDatabase>
#include "GetMy.h"
#include <QDirIterator>
#include <QDir>
#include "SettingsSerializer.h"

#define DEFAULT_FNT_SIZE 15

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

void FntSetting::on_hiraganaDropdown_activated(int index)
{
    currentHiraganFntIdx = index;
    SettingsSerializer::settings.setValue("FntSettings/HiraganaFntIdx", index);
    // TODO update fnt size of stocked
}

void FntSetting::on_KatakanaDropdown_activated(int index)
{
    currentKatakanaFntIdx = index;
    SettingsSerializer::settings.setValue("FntSettings/KatakanaFntIdx", index);
    // TODO update fnt size of stocked
}

void FntSetting::on_RomanjiDropdown_activated(int index)
{
    currentRomanjiFntIdx = index;
    SettingsSerializer::settings.setValue("FntSettings/RomanjiFntIdx", index);
    // TODO update fnt size of stocked
}

void FntSetting::on_HiraganaSizeSlider_actionTriggered(int size)
{
    // TODO update fnt size of stocked
    currentHiraganaSize = size;
    ui->HiraganaSizeValueLabel->setText(QString::number(size));
    SettingsSerializer::settings.setValue("FntSettings/HiraganaFntSize", size);
}

void FntSetting::on_KatakanaSizeSlider_actionTriggered(int size)
{
    // TODO update fnt size of stocked
    currentKatakanaSize = size;
    ui->KatakanaSizeValueLabel->setText(QString::number(size));
    SettingsSerializer::settings.setValue("FntSettings/KatakanaFntSize", size);
}

void FntSetting::on_RomanjiSizeSlider_actionTriggered(int size)
{
    // TODO update fnt size of stocked
    currentRomanjiSize = size;
    ui->RomanjiSizeValueLabel->setText(QString::number(size));
    SettingsSerializer::settings.setValue("FntSettings/RomanjiFntSize", size);
}
