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

    void InitializeSymbolSettingSection(SymbolsTables::SymbolsTableSection& symbolTableSection);

private:
    Ui::SymbolSettingSection *ui;
    QList<SymbolSettingEntry*> symbolSettingsEntries;
};

#endif // SYMBOLSETTINGSECTION_H
