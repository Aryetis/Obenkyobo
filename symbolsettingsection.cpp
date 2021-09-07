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

    int i=0;
    for (Symbol& symbol : symbolTableSection.Data()) // TODO now hardcode the case skip and fix the style
    {
        SymbolSettingEntry* symbolEntry = new SymbolSettingEntry();
        symbolEntry->InitializeSymbolSettingEntry(&symbol);
        symbolSettingsEntries.append(symbolEntry);

        if (i==36 || i==38) // leave empty space for (yi || ye), ugly but I don't care
            ++i;
        div_t entryPos = div(i++, symbolTableSection.ElementPerColumnInDisplaySet());
        ui->SymbolSettingSectionEntries->addWidget(symbolEntry, entryPos.quot, entryPos.rem);
    }

    for (int i=0; i < ui->SymbolSettingSectionEntries->columnCount(); ++i)
        ui->SymbolSettingSectionEntries->setColumnStretch(i, 1);
}

void SymbolSettingSection::on_SymbolSettingSectionCheckbox_clicked(bool checked)
{
    for (SymbolSettingEntry* symbol : symbolSettingsEntries)
        symbol->FakeClick(checked);
}
