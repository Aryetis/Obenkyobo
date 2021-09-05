#ifndef SYMBOLSETTINGENTRY_H
#define SYMBOLSETTINGENTRY_H

#include <QWidget>
#include "symbolstables.h"

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

private slots:
    void on_pushButton_clicked(bool checked);

private:
    Ui::SymbolSettingEntry *ui;
    Symbol* symbol;
};

#endif // SYMBOLSETTINGENTRY_H
