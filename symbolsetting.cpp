#include "symbolsetting.h"
#include "ui_symbolsetting.h"

SymbolSetting::SymbolSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolSet)
{
    ui->setupUi(this);
}

SymbolSetting::~SymbolSetting()
{
    delete ui;
}
