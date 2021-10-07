#include "symbolsetting.h"
#include "ui_symbolsetting.h"
#include "GetMy.h"
#include "symbolsettingsection.h"
#include "symbolstables.h"

#define SCROLLBAR_WIDTH 40

SymbolSetting::SymbolSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolSetting)
{
    ui->setupUi(this);

    SymbolsTables::HiraganaSymbolsTableFamily.InitializeSerializedVals();
    SymbolsTables::KatakanaSymbolsTableFamily.InitializeSerializedVals();

    GetMy::Instance().SetSymbolSettingWidget(this);
}

SymbolSetting::~SymbolSetting()
{
    delete ui;
}

void SymbolSetting::resizeEvent(QResizeEvent* event)
{
   QWidget::resizeEvent(event);
   setStyleSheet(QString("QScrollBar:vertical { width : %1px }").arg(SCROLLBAR_WIDTH));
//   setStyleSheet(QString("QScrollBar::handle:vertical { background-color: red; }"));
}

void SymbolSetting::InitializeSymbolSetting(SymbolFamilyEnum _symbolFamily)
{
    symbolFamily = _symbolFamily;
    std::vector<SymbolsTableSection>& symbolsTableFamilyTarget = (symbolFamily == SymbolFamilyEnum::hiragana)
            ? SymbolsTables::HiraganaSymbolsTableFamily.Data()
            : SymbolsTables::KatakanaSymbolsTableFamily.Data();

    qDeleteAll(symbolsTableSections);
    symbolsTableSections.clear();

    for (SymbolsTableSection& symbolTableSection : symbolsTableFamilyTarget )
    {
        SymbolSettingSection* symbolSettingSection = new SymbolSettingSection();
        symbolSettingSection->InitializeSymbolSettingSection(symbolTableSection);
        symbolsTableSections.append(symbolSettingSection);

        ui->SymbolSettingContainer->addWidget(symbolSettingSection);
    }
}
