#include "fntsetting.h"
#include "ui_fntsetting.h"
#include <QFontDatabase>

FntSetting::FntSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FntSetting)
{
    currentHiraganaFnt = QFont(":/HiraganaFonts/DotGothic16-Regular.ttf", QFont::Normal);
    currentKatakanaFnt = QFont(":/HiraganaFonts/DotGothic16-Regular.ttf", QFont::Normal);
    currentRomanjiFnt = QFont(":/HiraganaFonts/DotGothic16-Regular.ttf", QFont::Normal);

    InitializeFntSetting();

    ui->setupUi(this);
}

FntSetting::~FntSetting()
{
    delete ui;
}

// TODO now fix called multiple time / each time you enter the page ... duh
void FntSetting::InitializeFntSetting()
{
    for (QString fntName : hiraganaFonts)
        ui->hiraganaDropdown->addItem(fntName);
    for (QString fntName : katakanaFonts)
        ui->KatakanaDropdown->addItem(fntName);
    for (QString fntName : romanjiFonts)
        ui->RomanjiDropdown->addItem(fntName);
}

void FntSetting::RegisterHiraganaFont(std::string fntAddress)
{
    hiraganaFonts.emplace_back(GetFontName(fntAddress));
}

void FntSetting::RegisterKatakanaFont(std::string fntAddress)
{
    katakanaFonts.emplace_back(GetFontName(fntAddress));
}

void FntSetting::RegisterRomanjiFont(std::string fntAddress)
{
    romanjiFonts.emplace_back(GetFontName(fntAddress));
}

QString FntSetting::GetFontName(std::string fntAddress)
{
    int id = QFontDatabase::addApplicationFont(QString::fromStdString(fntAddress));
    return QFontDatabase::applicationFontFamilies(id).at(0);
    //QFont(name,QFont::Normal);
}

void FntSetting::on_hiraganaDropdown_activated(const QString &arg1)
{
    currentHiraganaFnt = QFont(arg1, QFont::Normal);
}


void FntSetting::on_KatakanaDropdown_activated(const QString &arg1)
{
    currentKatakanaFnt = QFont(arg1, QFont::Normal);
}

void FntSetting::on_RomanjiDropdown_activated(const QString &arg1)
{
    currentRomanjiFnt = QFont(arg1, QFont::Normal);
}
