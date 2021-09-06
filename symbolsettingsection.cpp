#include "symbolsettingsection.h"
#include "ui_symbolsettingsection.h"

SymbolSettingSection::SymbolSettingSection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolSettingSection)
{
    ui->setupUi(this);
}

SymbolSettingSection::~SymbolSettingSection()
{
    delete ui;
}

void SymbolSettingSection::InitializeSymbolSettingSection(SymbolsTables::SymbolsTableSection& symbolTableSection)
{
    qDeleteAll(symbolSettingsEntries);
    symbolSettingsEntries.clear();

    ui->SymbolSettingSectionName->setText(symbolTableSection.Name());

    for (Symbol symbol : symbolTableSection.Data()) // TODO now hardcode the case skip and fix the style
    {
        SymbolSettingEntry* symbolEntry = new SymbolSettingEntry();
        symbolEntry->InitializeSymbolSettingEntry(&symbol);
        symbolSettingsEntries.append(symbolEntry);

        ui->SymbolSettingSectionEntries->addWidget(symbolEntry);
    }
}
