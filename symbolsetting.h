#ifndef SYMBOLSET_H
#define SYMBOLSET_H

#include <QWidget>
#include "symbolstables.h"
#include "symbolsettingsection.h"

namespace Ui
{
    class SymbolSetting;
}

class SymbolSetting : public QWidget
{
    Q_OBJECT

public:
    explicit SymbolSetting(QWidget *parent = nullptr);
    ~SymbolSetting() override;

    void InitializeSymbolSetting(SymbolFamilyEnum _symbolFamily);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::SymbolSetting *ui;
    SymbolFamilyEnum symbolFamily;

    QList<SymbolSettingSection*> symbolsTableSections;
};

#endif // SYMBOLSET_H
