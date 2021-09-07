#include "symbolsettingentry.h"
#include "ui_symbolsettingentry.h"

SymbolSettingEntry::SymbolSettingEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolSettingEntry)
{
    ui->setupUi(this);
}

SymbolSettingEntry::~SymbolSettingEntry()
{
    delete ui;
}

void SymbolSettingEntry::InitializeSymbolSettingEntry(Symbol* _symbol)
{
    symbol = _symbol;

    ui->SymbolSettingEntryButton->setText(symbol->JP() + "\n" + symbol->Romanji());
}

void SymbolSettingEntry::on_SymbolSettingEntryButton_clicked(bool checked)
{
    symbol->SetEnabled(checked);
    ui->SymbolSettingEntryButton->update();
}
