#include "Src/symbolsetting.h"
#include "ui_symbolsetting.h"
#include "Src/GetMy.h"
#include "Src/symbolsettingsection.h"
#include "Src/symbolstables.h"
#include "Src/appsettings.h"
#include <QScrollBar>


SymbolSetting::SymbolSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolSetting)
{
    ui->setupUi(this);

    SymbolsTables::HiraganaSymbolsTableFamily.InitializeSerializedVals();
    SymbolsTables::KatakanaSymbolsTableFamily.InitializeSerializedVals();

    connect( ui->scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &SymbolSetting::OnScroll);

    GetMy::Instance().SetSymbolSettingWidget(this);
}

SymbolSetting::~SymbolSetting()
{
    delete ui;
}

void SymbolSetting::InitializeSymbolSetting(SymbolFamilyEnum _symbolFamily)
{
    symbolFamily = _symbolFamily;
    std::vector<SymbolsTableSection>& symbolsTableFamilyTarget = (symbolFamily == SymbolFamilyEnum::hiragana)
            ? SymbolsTables::HiraganaSymbolsTableFamily.Data()
            : SymbolsTables::KatakanaSymbolsTableFamily.Data();

    qDeleteAll(symbolsTableSections);
    symbolsTableSections.clear();

    for (SymbolsTableSection& symbolTableSection : symbolsTableFamilyTarget )
    {
        SymbolSettingSection* symbolSettingSection = new SymbolSettingSection();
        symbolSettingSection->InitializeSymbolSettingSection(symbolTableSection, symbolFamily);
        symbolsTableSections.append(symbolSettingSection);

        ui->SymbolSettingContainer->addWidget(symbolSettingSection);
    }

    ui->scrollArea->setFocus(); // force focus on scrollbar so it handles physical buttons
}

void SymbolSetting::OnScroll(int /*value*/)
{
// TODO refresh screen properly
}
