#include "symbolsetting.h"
#include "ui_symbolsetting.h"
#include "GetMy.h"
#include "symbolsettingsection.h"
#include "symbolstables.h"

#include <QScrollBar>

#define SCROLLBAR_WIDTH 40

SymbolSetting::SymbolSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SymbolSetting)
{
    ui->setupUi(this);

    SymbolsTables::HiraganaSymbolsTableFamily.InitializeSerializedVals();
    SymbolsTables::KatakanaSymbolsTableFamily.InitializeSerializedVals();

    ui->scrollArea->horizontalScrollBar()->setPageStep(50); // TODO now fix button step value slooooooow
    ui->scrollArea->verticalScrollBar()->setPageStep(10);

    GetMy::Instance().SetSymbolSettingWidget(this);
}

SymbolSetting::~SymbolSetting()
{
    delete ui;
}

void SymbolSetting::resizeEvent(QResizeEvent* event)
{
   QWidget::resizeEvent(event);
//   setStyleSheet(QString("QScrollBar:vertical { width : %1px }").arg(SCROLLBAR_WIDTH));

   // TODO now fix image not showing up for button
   setStyleSheet(
"QScrollBar                                                                                                                 "
"{ border: 2px solid grey; background: white; width: 40px; margin: 42px 0 42px 0; }                                         "
"QScrollBar::handle                                                                                                         "
"{ background: rgb(60,60,60); min-height: 20px; }                                                                                   "
"QScrollBar::add-line                                                                                                       "
"{ border: 2px solid grey; background: white; height: 40px; subcontrol-position: bottom; subcontrol-origin: margin; }       "
"QScrollBar::sub-line                                                                                                       "
"{ border: 2px solid grey; background: white; height: 40px; subcontrol-position: top; subcontrol-origin: margin; }          "
"QScrollBar::add-page, QScrollBar::sub-page                                                                                 "
"{ background: none; }                                                                                                      "
"                                                                                                                           "
"QScrollBar::up-arrow                                                                                                       "
"{                                                                                                                          "
"	image: url(:/pictures/upArrow.png);                                                                              "
"	border: 2px solid grey; width: 3px; height: 3px; background: none;                                                     "
"}                                                                                                                          "
"QScrollBar::down-arrow                                                                                                     "
"{                                                                                                                          "
"	image: url(:/pictures/downArrow.png);                                                                            "
"	border: 2px solid grey; width: 3px; height: 3px; background: none;                                                     "
"}                                                                                                                          "
"                                                                                                                           "
"QScrollBar:left-arrow                                                                                                      "
"{                                                                                                                          "
"	image: url(:/pictures/leftArrow.png);                                                                            "
"	border: 2px solid grey; width: 3px; height: 3px; background: none;                                                     "
"}                                                                                                                          "
"QScrollBar::right-arrow                                                                                                    "
"{                                                                                                                          "
"	image: url(:/pictures/rightArrow);                                                                               "
"	border: 2px solid grey; width: 3px; height: 3px; background: none;                                                     "
"}                                                                                                                          "

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
