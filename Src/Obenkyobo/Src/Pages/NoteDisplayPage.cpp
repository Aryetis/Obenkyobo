#include "Src/Pages/NoteDisplayPage.h"
#include "Src/GetMy.h"
#include "Src/Widgets/FileEntryWidget.h"
#include "Src/mainwindow.h"
#include <QScrollBar>
#include <ui_NoteDisplayPage.h>

NoteDisplayPage::NoteDisplayPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::NoteDisplayPage)
{
    ui->setupUi(this);

    QColor baseBackgroundColor = QApplication::palette().window().color();
    QString textAreaStylesheet = QString("background-color:rgb(%1,%2,%3)").arg(
        QString::number(baseBackgroundColor.red()),
        QString::number(baseBackgroundColor.green()),
        QString::number(baseBackgroundColor.blue()));
    ui->TextWidget->setStyleSheet(textAreaStylesheet);

    ui->TextWidget->setReadOnly(true);
    ui->TextWidget->setTextInteractionFlags(Qt::NoTextInteraction);

    GetMy::Instance().SetNoteDisplayPageInst(this);
}

NoteDisplayPage::~NoteDisplayPage()
{
    delete ui;
}

bool NoteDisplayPage::eventFilter(QObject *obj, QEvent *event)
{
    if ( obj == ui->NoteDisplayScrollArea->verticalScrollBar()
        && (event->type() == QEvent::Type::Show ||
            event->type() == QEvent::Type::Hide))
    {
        GetMy::Instance().MainWindowInst().AggressiveClearScreen();
        ui->NoteDisplayScrollArea->verticalScrollBar()->setFocus();

    }

    return false;
}

void NoteDisplayPage::Populate(NoteFileEntryWidget const* nfew)
{
    ui->NoteDisplayScrollArea->verticalScrollBar()->installEventFilter(this);

    if (!nfew->FileInfo().isFile())
    {
        std::cerr << "NoteDisplayPage::Populate with invalid file" << std::endl;
        return;
    }
    else
    {
        if (nfew->FileInfo().suffix() == "md")
        {
            ui->TextWidget->setText("");
            ui->TextWidget->setMarkdown(GetFileInString(nfew->FileInfo()));
        }
        else if (nfew->FileInfo().suffix() == "txt")
        {
            ui->TextWidget->setMarkdown("");
            ui->TextWidget->setText(GetFileInString(nfew->FileInfo()));
        }
        else
        {
            std::cerr << "NoteDisplayPage::Populate with invalid extension" << std::endl;
            return;
        }
    }
}

QString NoteDisplayPage::GetFileInString(QFileInfo const& fileInfo) const
{
    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cerr << "ERROR : Could not open file properly" << std::endl;
        return "ERROR : Could not open file properly";
    }
    else
    {
        QTextStream fs(&file);
        return fs.readAll();
    }
}
