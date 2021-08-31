#ifndef SYMBOLSET_H
#define SYMBOLSET_H

#include <QWidget>

namespace Ui
{
    class SymbolSet;
}

class SymbolSetting : public QWidget
{
    Q_OBJECT

public:
    explicit SymbolSetting(QWidget *parent = nullptr);
    ~SymbolSetting();

private:
    Ui::SymbolSet *ui;
};

#endif // SYMBOLSET_H
