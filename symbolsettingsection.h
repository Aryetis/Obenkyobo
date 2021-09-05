#ifndef SYMBOLSETTINGSECTION_H
#define SYMBOLSETTINGSECTION_H

#include <QWidget>

namespace Ui {
class SymbolSettingSection;
}

class SymbolSettingSection : public QWidget
{
    Q_OBJECT

public:
    explicit SymbolSettingSection(QWidget *parent = nullptr);
    ~SymbolSettingSection();

private:
    Ui::SymbolSettingSection *ui;
};

#endif // SYMBOLSETTINGSECTION_H
