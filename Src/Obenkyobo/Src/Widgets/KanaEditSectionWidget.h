#ifndef KANAEDITSECTIONWIDGET_H
#define KANAEDITSECTIONWIDGET_H

#include <QWidget>
#include "Src/Widgets/KanaEditEntryWidget.h"

namespace Ui
{
    class KanaEditSectionWidget;
}

class SymbolsTableSection;
class KanaEditSectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KanaEditSectionWidget(QWidget *parent = nullptr);
    ~KanaEditSectionWidget() override;

    void InitializeKanaSettingSectionWidget(SymbolsTableSection& symbolTableSection, KanaFamilyEnum symbolFamily);
    void UpdateSectionCheckboxTristate(bool update);

private slots:
    void on_KanaSectionCheckbox_clicked(bool checked);
    void InitializeCheckbox();

private:
    Ui::KanaEditSectionWidget *ui;
    QList<KanaEditEntryWidget*> symbolSettingsEntries;
    int nbrSymbolChecked;
};

#endif // KANAEDITSECTIONWIDGET_H
