#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>

namespace Ui
{
    class WelcomePage;
}

class WelcomePage : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = nullptr);
    ~WelcomePage() override;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::WelcomePage *ui;
};

#endif // WELCOMEPAGE_H
