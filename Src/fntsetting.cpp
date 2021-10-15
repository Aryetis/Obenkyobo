#include "Src/fntsetting.h"
#include "ui_fntsetting.h"
#include <QFontDatabase>
#include "Src/GetMy.h"
#include <QDirIterator>
#include <QDir>

#define DEFAULT_HIRAGANA_FNT_SIZE 60
#define DEFAULT_KATAKANA_FNT_SIZE 60
#define DEFAULT_ROMANJI_FNT_SIZE 40
#define DEFAULT_STEM_BOOST_SIZE 20

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

    currentHiraganFntIdx = settingsSerializer->value("FntSettings/HiraganaFntIdx", 0).toInt();
    currentKatakanaFntIdx = settingsSerializer->value("FntSettings/KatakanaFntIdx", 0).toInt();
    currentRomanjiFntIdx = settingsSerializer->value("FntSettings/RomanjiFntIdx", 0).toInt();
    ui->HiraganaFntDropdown->setCurrentIndex(currentHiraganFntIdx);
    ui->KatakanaFntDropdown->setCurrentIndex(currentKatakanaFntIdx);
    ui->RomanjiFntDropdown->setCurrentIndex(currentRomanjiFntIdx);

    currentHiraganaSize =  settingsSerializer->value("FntSettings/HiraganaFntSize", DEFAULT_HIRAGANA_FNT_SIZE).toInt();
    currentKatakanaSize =  settingsSerializer->value("FntSettings/KatakanaFntSize", DEFAULT_KATAKANA_FNT_SIZE).toInt();
    currentRomanjiSize =  settingsSerializer->value("FntSettings/RomanjiFntSize", DEFAULT_ROMANJI_FNT_SIZE).toInt();
    ui->HiraganaSizeValueLabel->setText(QString::number(currentHiraganaSize));
    ui->HiraganaSizeSlider->setValue(currentHiraganaSize);
    ui->KatakanaSizeValueLabel->setText(QString::number(currentKatakanaSize));
    ui->KatakanaSizeSlider->setValue(currentKatakanaSize);
    ui->RomanjiSizeValueLabel->setText(QString::number(currentRomanjiSize));
    ui->RomanjiSizeSlider->setValue(currentRomanjiSize);

    stemBoostSize =  settingsSerializer->value("FntSettings/stemBoostSize", DEFAULT_STEM_BOOST_SIZE).toInt();
    ui->BoostStemSlider->setValue(stemBoostSize);
}

void FntSetting::RegisterHiraganaFont(QString fntAddress)
{
    hiraganaFonts.emplace_back(GetFont(fntAddress, fntTypeEnum::hiragana));
}

void FntSetting::RegisterKatakanaFont(QString fntAddress)
{
    katakanaFonts.emplace_back(GetFont(fntAddress, fntTypeEnum::katakana));
}

void FntSetting::RegisterRomanjiFont(QString fntAddress)
{
    romanjiFonts.emplace_back(GetFont(fntAddress, fntTypeEnum::romanji));
}

QFont FntSetting::GetFont(QString fntAddress, fntTypeEnum type)
{
    int id = QFontDatabase::addApplicationFont(fntAddress);
    QString name = QFontDatabase::applicationFontFamilies(id).at(0);
    switch (type)
    {
        case fntTypeEnum::hiragana :
            return QFont(name, DEFAULT_HIRAGANA_FNT_SIZE);
        case fntTypeEnum::katakana :
            return QFont(name, DEFAULT_KATAKANA_FNT_SIZE);
        case fntTypeEnum::romanji :
            return QFont(name, DEFAULT_ROMANJI_FNT_SIZE);
    }

    assert(false);
}

void FntSetting::on_HiraganaFntDropdown_activated(int index)
{
    currentHiraganFntIdx = index;
    settingsSerializer->setValue("FntSettings/HiraganaFntIdx", index);
    hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)].setFamily(
                ui->HiraganaFntDropdown->itemText(currentHiraganFntIdx));
    hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)].setPixelSize(currentHiraganaSize);
}

void FntSetting::on_KatakanaFntDropdown_activated(int index)
{
    currentKatakanaFntIdx = index;
    settingsSerializer->setValue("FntSettings/KatakanaFntIdx", index);
    katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)].setFamily(
                ui->KatakanaFntDropdown->itemText(currentKatakanaFntIdx));
    katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)].setPixelSize(currentKatakanaSize);

}

void FntSetting::on_RomanjiFntDropdown_activated(int index)
{
    currentRomanjiFntIdx = index;
    settingsSerializer->setValue("FntSettings/RomanjiFntIdx", index);
    romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)].setFamily(
                ui->RomanjiFntDropdown->itemText(currentRomanjiFntIdx));
    romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)].setPixelSize(currentRomanjiSize);
}

void FntSetting::on_HiraganaSizeSlider_valueChanged(int size)
{
    currentHiraganaSize = size;
    ui->HiraganaSizeValueLabel->setText(QString::number(size));
    settingsSerializer->setValue("FntSettings/HiraganaFntSize", size);
    hiraganaFonts[static_cast<std::vector<QFont>::size_type>(currentHiraganFntIdx)].setPixelSize(size);
}

void FntSetting::on_KatakanaSizeSlider_valueChanged(int size)
{
    currentKatakanaSize = size;
    ui->KatakanaSizeValueLabel->setText(QString::number(size));
    settingsSerializer->setValue("FntSettings/KatakanaFntSize", size);
    katakanaFonts[static_cast<std::vector<QFont>::size_type>(currentKatakanaFntIdx)].setPixelSize(size);
}

void FntSetting::on_RomanjiSizeSlider_valueChanged(int size)
{
    currentRomanjiSize = size;
    ui->RomanjiSizeValueLabel->setText(QString::number(size));
    settingsSerializer->setValue("FntSettings/RomanjiFntSize", size);
    romanjiFonts[static_cast<std::vector<QFont>::size_type>(currentRomanjiFntIdx)].setPixelSize(size);
}

void FntSetting::on_BoostStemSlider_valueChanged(int size)
{
    stemBoostSize = size;
    ui->BoostStemValueLabel->setText(QString::number(size));
    settingsSerializer->setValue("FntSettings/stemBoostSize", size);
}
