#include "Src/Pages/KanaEditPage.h"
#include "ui_KanaEditPage.h"
#include "Src/GetMy.h"
#include "Src/Widgets/KanaEditSectionWidget.h"
#include "Src/KanasTables.h"
#include "Src/Pages/AppSettingsPage.h"
#include <QScrollBar>
#include "Src/mainwindow.h"


KanaEditPage::KanaEditPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanaEditPage)
{
    ui->setupUi(this);

    KanasTables::HiraganaSymbolsTableFamily.InitializeSerializedVals();
    KanasTables::KatakanaSymbolsTableFamily.InitializeSerializedVals();

    connect( ui->scrollArea->verticalScrollBar(), &QScrollBar::sliderReleased, this, &KanaEditPage::OnSliderReleased);
    connect( ui->scrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &KanaEditPage::OnValueChanged);

    ui->scrollArea->verticalScrollBar()->installEventFilter(this);

    GetMy::Instance().SetKanaEditPageInst(this);
}

KanaEditPage::~KanaEditPage()
{
    delete ui;
}

void KanaEditPage::InitializeKanaPage(KanaFamilyEnum _symbolFamily)
{
    kanaFamily = _symbolFamily;
    std::vector<SymbolsTableSection>& symbolsTableFamilyTarget = (kanaFamily == KanaFamilyEnum::hiragana)
            ? KanasTables::HiraganaSymbolsTableFamily.Data()
            : KanasTables::KatakanaSymbolsTableFamily.Data();

    qDeleteAll(symbolsTableSections);
    symbolsTableSections.clear();

    for (SymbolsTableSection& symbolTableSection : symbolsTableFamilyTarget )
    {
        KanaEditSectionWidget* symbolSettingSection = new KanaEditSectionWidget();
        symbolSettingSection->InitializeKanaSettingSectionWidget(symbolTableSection, kanaFamily);
        symbolsTableSections.append(symbolSettingSection);

        ui->SymbolSettingContainer->addWidget(symbolSettingSection);
    }

    ui->scrollArea->setFocus(); // force focus on scrollbar so it handles physical buttons
}

bool KanaEditPage::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->scrollArea->verticalScrollBar() && event->type() == QEvent::Type::Show)
        GetMy::Instance().MainWindowInst().AggressiveClearScreen();

    return false;
}

void KanaEditPage::OnSliderReleased() const
{
    GetMy::Instance().MainWindowInst().AggressiveClearScreen();
}

void KanaEditPage::OnValueChanged(int /*value*/) const
{
    if (!ui->scrollArea->verticalScrollBar()->isSliderDown())
        GetMy::Instance().MainWindowInst().AggressiveClearScreen();
}
