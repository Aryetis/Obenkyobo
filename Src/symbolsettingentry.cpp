#include "Src/symbolsettingentry.h"
#include "ui_symbolsettingentry.h"

#include "GetMy.h"
#include "fntsetting.h"

SymbolSettingEntry::SymbolSettingEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolSettingEntry)
{
    ui->setupUi(this);
    ui->SymbolSettingEntryButton->setStyleSheet("QPushButton:checked { background-color: rgb(10, 10, 10); color : rgb(255, 255, 255); }");
}

SymbolSettingEntry::~SymbolSettingEntry()
{
    delete ui;
}

void SymbolSettingEntry::InitializeSymbolSettingEntry(Symbol* _symbol, ::SymbolFamilyEnum symbolFamily)
{
    symbol = _symbol;

    ui->SymbolSettingEntryButton->setSymbolFamilyEnum(symbolFamily);
    ui->SymbolSettingEntryButton->setJpText(symbol->JP());
    ui->SymbolSettingEntryButton->setRmjText(symbol->Romanji());
    ui->SymbolSettingEntryButton->setLearningState(symbol->LearningState());
    ui->SymbolSettingEntryButton->setChecked(symbol->Enabled());
}

void SymbolSettingEntry::on_SymbolSettingEntryButton_clicked(bool checked)
{
    std::cout << "LOG : SymbolSettingEntry::on_SymbolSettingEntryButton_clicked()" << std::endl;
    symbol->Enabled(checked);
}

void SymbolSettingEntry::FakeClick(bool checked)
{
    symbol->Enabled(checked);
    ui->SymbolSettingEntryButton->setChecked(checked);
}
