#include "Src/symbolsettingsection.h"
#include "ui_symbolsettingsection.h"

SymbolSettingSection::SymbolSettingSection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolSettingSection)
{
    ui->setupUi(this);

    ui->SymbolSettingSectionCheckbox->setStyleSheet(
                QString(QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                        .arg((int)(GetMy::Instance().Descriptor().width*0.075f)))
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

    nbrSymbolChecked = 0;
    int i=0;
    for (Symbol& symbol : symbolTableSection.Data())
    {
        if (i==36 || i==38) // leave empty space for (yi || ye), ugly but I don't care
            ++i;

        SymbolSettingEntry* symbolEntry = new SymbolSettingEntry(this);
        symbolEntry->InitializeSymbolSettingEntry(&symbol, symbolFamily);
        symbolSettingsEntries.append(symbolEntry);
        if (symbolEntry->IsEnabled())
            ++nbrSymbolChecked;

        div_t entryPos = div(i++, symbolTableSection.ElementPerColumnInDisplaySet());
        ui->SymbolSettingSectionEntries->addWidget(symbolEntry, entryPos.quot, entryPos.rem);
    }
    InitializeCheckbox();
}

void SymbolSettingSection::InitializeCheckbox()
{
    if (nbrSymbolChecked >= symbolSettingsEntries.size())
        ui->SymbolSettingSectionCheckbox->setCheckState(Qt::CheckState::Checked);
    else if (nbrSymbolChecked <= 0)
        ui->SymbolSettingSectionCheckbox->setCheckState(Qt::CheckState::Unchecked);
    else
        ui->SymbolSettingSectionCheckbox->setCheckState(Qt::CheckState::PartiallyChecked);
}

void SymbolSettingSection::UpdateSectionCheckboxTristate(bool update)
{
    nbrSymbolChecked += update ? 1 : -1;
    InitializeCheckbox();
}

void SymbolSettingSection::on_SymbolSettingSectionCheckbox_clicked(bool checked)
{
    nbrSymbolChecked = (checked) ? symbolSettingsEntries.size() : 0;
    ui->SymbolSettingSectionCheckbox->setCheckState(checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    for (SymbolSettingEntry* symbol : symbolSettingsEntries)
        symbol->FakeClick(checked);
}
