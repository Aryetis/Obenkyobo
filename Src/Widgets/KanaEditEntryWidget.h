#ifndef KANAEDITENTRYWIDGET_H
#define KANAEDITENTRYWIDGET_H

#include <QWidget>
#include "Src/DefinesLand.h"
#include "Src/QcmDataEntryKana.h"

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

    void InitializeSymbolSettingEntry(QcmDataEntryKana* _kana, KanaFamilyEnum kanaFamily);
    void FakeClick(bool b);
    bool IsEnabled() const { return kana->IsEnabled(); }

private slots:
    void on_KanaEntryButton_clicked(bool checked);

private:
    Ui::KanaEditEntryWidget *ui; // holds a KanaEditButtonWidget, TODO : simplify this and merge them
    QcmDataEntryKana* kana;
};

#endif // KANAEDITENTRYWIDGET_H
