#include "Src/symbolsetting.h"
#include "ui_symbolsetting.h"
#include "Src/GetMy.h"
#include "Src/symbolsettingsection.h"
#include "Src/symbolstables.h"

#include <QScrollBar>


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
    setStyleSheet
    (
        "QScrollBar                                                                                                "
        "{ border: 2px solid grey; background: white; width: 40px; margin: 40px 0 40px 0; }                         "
        "QScrollBar::handle                                                                                         "
        "{ background: rgb(60,60,60); min-height: 20px; }                                                           "
        "QScrollBar::add-line                                                                                       "
        "{ border: none; background: white; height: 40px; subcontrol-position: bottom; subcontrol-origin: margin; } "
        "QScrollBar::sub-line                                                                                       "
        "{ border: none; background: white; height: 40px; subcontrol-position: top; subcontrol-origin: margin; }    "
        "QScrollBar::add-page, QScrollBar::sub-page                                                                 "
        "{ background: none; }                                                                                      "
        "                                                                                                           "
        "QScrollBar::up-arrow                                                                                       "
        "{                                                                                                          "
        "	image: url(:/pictures/Arrows/upArrow.png);                                                            "
        "	border: none; width: 30px; height: 30px; background: none;                                              "
        "}                                                                                                          "
        "QScrollBar::down-arrow                                                                                     "
        "{                                                                                                          "
        "	image: url(:/pictures/Arrows/downArrow.png);                                                          "
        "	border: 2none; width: 30px; height: 30px; background: none;                                             "
        "}                                                                                                          "
        "QScrollBar:left-arrow                                                                                      "
        "{                                                                                                          "
        "	image: url(:/pictures/Arrows/leftArrow.png);                                                          "
        "	border: none; width: 30px; height: 30px; background: none;                                              "
        "}                                                                                                          "
        "QScrollBar::right-arrow                                                                                    "
        "{                                                                                                          "
        "	image: url(:/pictures/Arrows/rightArrow.png);                                                         "
        "	border: none; width: 30px; height: 30px; background: none;                                              "
        "}                                                                                                          "
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
        symbolSettingSection->InitializeSymbolSettingSection(symbolTableSection, symbolFamily);
        symbolsTableSections.append(symbolSettingSection);

        ui->SymbolSettingContainer->addWidget(symbolSettingSection);
    }
}