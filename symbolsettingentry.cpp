#include "symbolsettingentry.h"
#include "ui_symbolsettingentry.h"

SymbolSettingEntry::SymbolSettingEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolSettingEntry)
{
    ui->setupUi(this);
    ui->SymbolSettingEntryButton->setStyleSheet("QPushButton:checked { background-color: rgb(100, 100, 100); }");
}

SymbolSettingEntry::~SymbolSettingEntry()
{
    delete ui;
}

void SymbolSettingEntry::InitializeSymbolSettingEntry(Symbol* _symbol)
{
    symbol = _symbol;

    ui->SymbolSettingEntryButton->setText(symbol->JP() + "\n" + symbol->Romanji());
    ui->SymbolSettingEntryButton->setChecked(symbol->Enabled());
}

void SymbolSettingEntry::on_SymbolSettingEntryButton_clicked(bool checked)
{
    symbol->Enabled(checked);
}

void SymbolSettingEntry::FakeClick(bool checked)
{
    symbol->Enabled(checked);
    ui->SymbolSettingEntryButton->setChecked(checked);
}
