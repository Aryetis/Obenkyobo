#ifndef NOTEDISPLAYPAGE_H
#define NOTEDISPLAYPAGE_H
#include <QWidget>
#include <QColor>
#include <QRegularExpression>

namespace Ui
{
    class NoteDisplayPage;
}

class CustomSyntaxHighlighter;

class NoteFileEntryWidget;
class QFileInfo;
class NoteDisplayPage : public QWidget
{
    Q_OBJECT

public:
    explicit NoteDisplayPage(QWidget *parent = nullptr);
    ~NoteDisplayPage() override;

    void Populate(NoteFileEntryWidget const* nfew);

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    QString GetFileInString(QFileInfo const& fileInfo, bool applyColorTags = false) const;

    Ui::NoteDisplayPage *ui;
};

#endif // NOTEDISPLAYPAGE_H
