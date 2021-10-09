#ifndef SYMBOLSETTINGSECTION_H
#define SYMBOLSETTINGSECTION_H

#include <QWidget>
#include "symbolstables.h"
#include "symbolsettingentry.h"

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

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::SymbolSettingSection *ui;
    QList<SymbolSettingEntry*> symbolSettingsEntries;

    int checkboxAdjustedSize;
};

#endif // SYMBOLSETTINGSECTION_H
