#include "Src/Widgets/KanaEditEntryWidget.h"
#include "ui_KanaEditEntryWidget.h"

#include "Src/GetMy.h"
#include "Src/Pages/FntSettingsPage.h"

KanaEditEntryWidget::KanaEditEntryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanaEditEntryWidget)
{
    ui->setupUi(this);
    ui->KanaEntryButton->setStyleSheet("QPushButton:checked { background-color: rgb(10, 10, 10); color : rgb(255, 255, 255); }");
}

KanaEditEntryWidget::~KanaEditEntryWidget()
{
    delete ui;
}

void KanaEditEntryWidget::InitializeSymbolSettingEntry(Kana* _kana, KanaFamilyEnum kanaFamily)
{
    kana = _kana;

    ui->KanaEntryButton->setSymbolFamilyEnum(kanaFamily);
    ui->KanaEntryButton->setJpText(kana->JP());
    ui->KanaEntryButton->setRmjText(kana->Romanji());
    ui->KanaEntryButton->setLearningState(kana->LearningState());
    ui->KanaEntryButton->setChecked(kana->IsEnabled());
}

void KanaEditEntryWidget::on_SymbolSettingEntryButton_clicked(bool checked)
{
    std::cout << "LOG : SymbolSettingEntry::on_SymbolSettingEntryButton_clicked()" << std::endl;
    kana->Enabled(checked);
    KanaEditSectionWidget* symbolSettingSectionParent = static_cast<KanaEditSectionWidget*>(parent());
    if (symbolSettingSectionParent)
        symbolSettingSectionParent->UpdateSectionCheckboxTristate(checked);
}

void KanaEditEntryWidget::FakeClick(bool checked)
{
    kana->Enabled(checked);
    ui->KanaEntryButton->setChecked(checked);
}
