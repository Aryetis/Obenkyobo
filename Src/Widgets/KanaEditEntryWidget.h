#ifndef KANAEDITENTRYWIDGET_H
#define KANAEDITENTRYWIDGET_H

#include <QWidget>
#include "Src/DefinesLand.h"
#include "Src/QcmDataEntry.h"

namespace Ui
{
    class KanaEditEntryWidget;
}

class KanaEditEntryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KanaEditEntryWidget(QWidget *parent = nullptr);
    ~KanaEditEntryWidget() override;

    void InitializeSymbolSettingEntry(QcmDataEntry* _kana, KanaFamilyEnum kanaFamily);
    void FakeClick(bool b);
    bool IsEnabled() const { return kana->IsEnabled(); }

private slots:
    void on_KanaEntryButton_clicked(bool checked);

private:
    Ui::KanaEditEntryWidget *ui; // holds a KanaEditButtonWidget, TODO : simplify this and merge them
    QcmDataEntry* kana;
};

#endif // KANAEDITENTRYWIDGET_H
