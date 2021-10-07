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

   setStyleSheet(

               "QScrollBar {                                                    "
               " border: 2px solid grey;                                              "
               " background: white;                                                 "
               " width: 40px;                                                         "
               " margin: 42px 0 42px 0;                                               "
               "}                                                                        "
               "QScrollBar::handle {                                            "
               " background: black;                                                   "
               " min-height: 20px;                                                    "
               "}                                                                        "
               "QScrollBar::add-line {                                          "
               " border: 2px solid grey;                                              "
               " background: white;                                                 "
               " height: 40px;                                                        "
               " subcontrol-position: bottom;                                         "
               " subcontrol-origin: margin;                                           "
               "}                                                                        "
               "																	  "
               "QScrollBar::sub-line {                                          "
               " border: 2px solid grey;                                              "
               " background: white;                                                 "
               " height: 40px;                                                        "
               " subcontrol-position: top;                                            "
               " subcontrol-origin: margin;                                           "
               "}                                                                        "
               "QScrollBar::up-arrow, QScrollBar::down-arrow, "
               "QScrollBar:left-arrow, QScrollBar::right-arrow {         "
               " border: 2px solid grey;                                              "
               " width: 3px;                                                          "
               " height: 3px;                                                         "
               " background: white;                                                   "
               "}                                                                        "
               "																	  "
               "QScrollBar::add-page, QScrollBar::sub-page {           "
               " background: none;                                                    "
               "}             "





);

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
