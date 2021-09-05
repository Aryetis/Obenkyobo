#ifndef SYMBOLSET_H
#define SYMBOLSET_H
#include <QWidget>
#include "symbolstables.h"

namespace Ui
{
    class SymbolSetting;
}

class SymbolSetting : public QWidget
{
    Q_OBJECT

    enum SymbolFamilyEnum
    {
        hiragana,
        katakana
    };

public:
    explicit SymbolSetting(QWidget *parent = nullptr);
    ~SymbolSetting() override;

    void InitializeSymbolSetting(SymbolFamilyEnum _symbolFamily);

private:
    Ui::SymbolSetting *ui;
    SymbolFamilyEnum symbolFamily;
};

#endif // SYMBOLSET_H
