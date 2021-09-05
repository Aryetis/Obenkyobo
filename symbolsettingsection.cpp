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
// TODO now initialize each symbolsettingentry
