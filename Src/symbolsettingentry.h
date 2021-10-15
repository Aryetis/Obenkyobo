#ifndef SYMBOLSETTINGENTRY_H
#define SYMBOLSETTINGENTRY_H

#include <QWidget>
#include "Src/symbolstables.h"

namespace Ui
{
    class SymbolSettingEntry;
}

class SymbolSettingEntry : public QWidget
{
    Q_OBJECT

public:
    explicit SymbolSettingEntry(QWidget *parent = nullptr);
    ~SymbolSettingEntry() override;

    void InitializeSymbolSettingEntry(Symbol* _symbol, SymbolFamilyEnum symbolFamily);
    void FakeClick(bool b);

private slots:
    void on_SymbolSettingEntryButton_clicked(bool checked);

private:
    Ui::SymbolSettingEntry *ui;
    Symbol* symbol;
};

#endif // SYMBOLSETTINGENTRY_H
