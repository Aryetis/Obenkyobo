#include "Src/Widgets/KanaEditSectionWidget.h"
#include "ui_KanaEditSectionWidget.h"

KanaEditSectionWidget::KanaEditSectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KanaEditSectionWidget)
{
    ui->setupUi(this);

    ui->KanaSectionCheckbox->setStyleSheet(
                QString(QString("QCheckBox::indicator { width: %1px; height: %1px;}")
                        .arg((int)(GetMy::Instance().Descriptor().width*0.075f)))
    );
}

KanaEditSectionWidget::~KanaEditSectionWidget()
{
    delete ui;
}


void KanaEditSectionWidget::InitializeKanaSettingSectionWidget(SymbolsTableSection& symbolTableSection, KanaFamilyEnum symbolFamily)
{
    qDeleteAll(symbolSettingsEntries);
    symbolSettingsEntries.clear();

    ui->KanaSectionName->setText(symbolTableSection.Name());

    nbrSymbolChecked = 0;
    int i=0;
    for (Kana& symbol : symbolTableSection.Data())
    {
        if (i==36 || i==38) // leave empty space for (yi || ye), ugly but I don't care
            ++i;

        KanaEditEntryWidget* kanaEntry = new KanaEditEntryWidget(this);
        kanaEntry->InitializeSymbolSettingEntry(&symbol, symbolFamily);
        symbolSettingsEntries.append(kanaEntry);
        if (kanaEntry->IsEnabled())
            ++nbrSymbolChecked;

        div_t entryPos = div(i++, symbolTableSection.ElementPerColumnInDisplaySet());
        ui->KanaSectionEntries->addWidget(kanaEntry, entryPos.quot, entryPos.rem);
    }
    InitializeCheckbox();
}

void KanaEditSectionWidget::InitializeCheckbox()
{
    if (nbrSymbolChecked >= symbolSettingsEntries.size())
        ui->KanaSectionCheckbox->setCheckState(Qt::CheckState::Checked);
    else if (nbrSymbolChecked <= 0)
        ui->KanaSectionCheckbox->setCheckState(Qt::CheckState::Unchecked);
    else
        ui->KanaSectionCheckbox->setCheckState(Qt::CheckState::PartiallyChecked);
}

void KanaEditSectionWidget::UpdateSectionCheckboxTristate(bool update)
{
    nbrSymbolChecked += update ? 1 : -1;
    InitializeCheckbox();
}

void KanaEditSectionWidget::on_KanaSectionCheckbox_clicked(bool checked)
{
    nbrSymbolChecked = (checked) ? symbolSettingsEntries.size() : 0;
    ui->KanaSectionCheckbox->setCheckState(checked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    for (KanaEditEntryWidget* kana : symbolSettingsEntries)
        kana->FakeClick(checked);
}
