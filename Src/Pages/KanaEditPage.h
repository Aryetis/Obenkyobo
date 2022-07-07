#ifndef SYMBOLSET_H
#define SYMBOLSET_H

#include <QWidget>
#include "Src/KanasTables.h"
#include "Src/Widgets/KanaEditSectionWidget.h"

namespace Ui
{
    class KanaEditPage;
}

class KanaEditPage : public QWidget
{
    Q_OBJECT

public:
    explicit KanaEditPage(QWidget *parent = nullptr);
    ~KanaEditPage() override;

    void InitializeKanaPage(KanaFamilyEnum _symbolFamily);

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    Ui::KanaEditPage *ui;
    KanaFamilyEnum kanaFamily;

    void OnSliderReleased() const;
    void OnValueChanged(int /*value*/) const;

    QList<KanaEditSectionWidget*> symbolsTableSections;
};

#endif // SYMBOLSET_H
