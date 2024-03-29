#include <iostream>
#include "Src/Widgets/KanaEditEntryWidget.h"
#include "ui_KanaEditEntryWidget.h"
#include "Src/Widgets/KanaEditSectionWidget.h"

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

void KanaEditEntryWidget::InitializeSymbolSettingEntry(QcmDataEntryKana* _kana, KanaFamilyEnum kanaFamily)
{
    kana = _kana;

    ui->KanaEntryButton->setSymbolFamilyEnum(kanaFamily);
    ui->KanaEntryButton->setJpText(*kana->Kanas());
    ui->KanaEntryButton->setRmjText(*kana->Romanji());
    ui->KanaEntryButton->setLearningState(kana->LearningScore());
    ui->KanaEntryButton->setChecked(kana->IsEnabled());
}

void KanaEditEntryWidget::on_KanaEntryButton_clicked(bool checked)
{
    std::cout << "LOG: SymbolSettingEntry::on_SymbolSettingEntryButton_clicked()" << std::endl;
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
