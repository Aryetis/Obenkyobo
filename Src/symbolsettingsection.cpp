#include "Src/symbolsettingsection.h"
#include "ui_symbolsettingsection.h"

SymbolSettingSection::SymbolSettingSection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolSettingSection)
{
    ui->setupUi(this);

    ui->SymbolSettingSectionCheckbox->setStyleSheet(
                QString("QCheckBox::indicator { width: %1px; height: %1px;}").arg
                    (GetMy::Instance().Descriptor().width/12)
    );
}

SymbolSettingSection::~SymbolSettingSection()
{
    delete ui;
}


void SymbolSettingSection::InitializeSymbolSettingSection(SymbolsTableSection& symbolTableSection, SymbolFamilyEnum symbolFamily)
{
    qDeleteAll(symbolSettingsEntries);
    symbolSettingsEntries.clear();

    ui->SymbolSettingSectionName->setText(symbolTableSection.Name());

    int i=0;
    for (Symbol& symbol : symbolTableSection.Data())
    {
        SymbolSettingEntry* symbolEntry = new SymbolSettingEntry();
        symbolEntry->InitializeSymbolSettingEntry(&symbol, symbolFamily);
        symbolSettingsEntries.append(symbolEntry);

        if (i==36 || i==38) // leave empty space for (yi || ye), ugly but I don't care
            ++i;
        div_t entryPos = div(i++, symbolTableSection.ElementPerColumnInDisplaySet());
        ui->SymbolSettingSectionEntries->addWidget(symbolEntry, entryPos.quot, entryPos.rem);
    }
}

void SymbolSettingSection::on_SymbolSettingSectionCheckbox_clicked(bool checked)
{
    for (SymbolSettingEntry* symbol : symbolSettingsEntries)
        symbol->FakeClick(checked);
}
