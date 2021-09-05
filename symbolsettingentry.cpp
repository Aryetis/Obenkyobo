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

void SymbolSettingEntry::on_pushButton_clicked(bool checked)
{
    symbol->SetEnabled(checked);
    ui->SymbolSettingEntryButton->setAutoFillBackground(true);
    ui->SymbolSettingEntryButton->setPalette(checked ? Symbol::EnabledPal : Symbol::DisabledPal);
    ui->SymbolSettingEntryButton->update();
}
