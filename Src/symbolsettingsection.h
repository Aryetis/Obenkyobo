#ifndef SYMBOLSETTINGSECTION_H
#define SYMBOLSETTINGSECTION_H

#include <QWidget>
#include "Src/symbolstables.h"
#include "Src/symbolsettingentry.h"

namespace Ui {
class SymbolSettingSection;
}

class SymbolSettingSection : public QWidget
{
    Q_OBJECT

public:
    explicit SymbolSettingSection(QWidget *parent = nullptr);
    ~SymbolSettingSection() override;

    void InitializeSymbolSettingSection(SymbolsTableSection& symbolTableSection, SymbolFamilyEnum symbolFamily);

private slots:
    void on_SymbolSettingSectionCheckbox_clicked(bool checked);

private:
    Ui::SymbolSettingSection *ui;
    QList<SymbolSettingEntry*> symbolSettingsEntries;
};

#endif // SYMBOLSETTINGSECTION_H
