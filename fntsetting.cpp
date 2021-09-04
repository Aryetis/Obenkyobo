#include "fntsetting.h"
#include "ui_fntsetting.h"
#include <QFontDatabase>
#include "GetMy.h"
#include <QDirIterator>
#include <QDir>

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
    QDirIterator it(":/HiraganaFonts",  {"*.ttf"}, QDir::Files);
    while (it.hasNext())
        RegisterHiraganaFont(it.next());

    // TODO import proper fonts for those
    RegisterKatakanaFont(":/HiraganaFonts/DotGothic16-Regular.ttf");
    RegisterRomanjiFont(":/HiraganaFonts/DotGothic16-Regular.ttf");

    for (QString fntName : hiraganaFontsNames)
        ui->hiraganaDropdown->addItem(fntName);
    for (QString fntName : katakanaFontsNames)
        ui->KatakanaDropdown->addItem(fntName);
    for (QString fntName : romanjiFontsNames)
        ui->RomanjiDropdown->addItem(fntName);

    ui->hiraganaDropdown->setCurrentIndex(0);
    ui->KatakanaDropdown->setCurrentIndex(0);
    ui->RomanjiDropdown->setCurrentIndex(0);

    // TODO use .cfg to store those
    currentHiraganaFnt = QFont(ui->hiraganaDropdown->itemText(0));
    currentKatakanaFnt = QFont(ui->KatakanaDropdown->itemText(0));
    currentRomanjiFnt = QFont(ui->RomanjiDropdown->itemText(0));
}

void FntSetting::RegisterHiraganaFont(QString fntAddress)
{
    hiraganaFontsNames.emplace_back(GetFontName(fntAddress));
}

void FntSetting::RegisterKatakanaFont(QString fntAddress)
{
    katakanaFontsNames.emplace_back(GetFontName(fntAddress));
}

void FntSetting::RegisterRomanjiFont(QString fntAddress)
{
    romanjiFontsNames.emplace_back(GetFontName(fntAddress));
}

QString FntSetting::GetFontName(QString fntAddress)
{
    int id = QFontDatabase::addApplicationFont(fntAddress);
    return QFontDatabase::applicationFontFamilies(id).at(0);
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
