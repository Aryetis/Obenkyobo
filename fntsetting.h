#ifndef FNTSETTING_H
#define FNTSETTING_H

#include <QWidget>

namespace Ui {
class FntSetting;
}

class FntSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FntSetting(QWidget *parent = nullptr);
    ~FntSetting();

private:
    Ui::FntSetting *ui;
};

#endif // FNTSETTING_H
