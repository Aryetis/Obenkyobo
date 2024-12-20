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
    // Adding margin for stuff like ordered list that can get printed out of screen otherwise
    ui->TextWidget->document()->setDocumentMargin((GetMy::Instance().Descriptor().height/100.0f)*1.5f);
    QString textAreaStylesheet = QString("background-color:rgb(%1,%2,%3);").arg(
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
            ui->TextWidget->setMarkdown(GetFileInString(nfew->FileInfo(), true));
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

QString NoteDisplayPage::GetFileInString(QFileInfo const& fileInfo, bool applyColorTags /*= false*/) const
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
        QString content {fs.readAll()};

        if (content.trimmed() == "")
        {
            return "FILE IS EMPTY :(";
        }
        else if(applyColorTags)
        {
            QVector<QString> colorTagStack;
            // hard coding some colors to make sure I don't catch other things by mistake
            QRegularExpression regex("<(azure|beige|black|blue|blueviolet|brown|"
                                     "chocolate|coral|crimson|cyan|darkblue|darkcyan|darkgray|darkgreen|darkgrey|darkmagenta|darkorange"
                                     "|darkviolet|fuchsia|gold|gray|green|grey|indigo|lavender|lightblue|lightcyan|lightgray|lightgreen"
                                     "|lightgrey|lightpink|lightyellow|lime|magenta|navy|orange|pink|purple|red|salmon|silver|turquoise"
                                     "|violet|white|yellow)>(.*?)(</\\1>°");
                                     //"|violet|white|yellow)>([\\s\\S]*?)</\\1>");


            // QRegularExpression regex("[<(.*?)>|(\\r?\\n)|(.*?)|</(.*?)>]");
            // QRegularExpressionMatchIterator i = regex.globalMatch(content);
            // int offset = 0;
            // while (i.hasNext())
            // {
            //     QRegularExpressionMatch match = i.next();

            //     content = content.replace(regex, "<font color='\\1'>\\2</font>");

            //     QString replacement = "<font color='" + match.captured(1) + "'>" + matchedContent.toUpper() + "</font>";
            //     result.replace(match.capturedStart(2) + offset, match.capturedLength(2), replacement);
            //     offset += replacement.length() - matchedContent.length();
            // }

            // Keep replacing as long as there are matches
            while (regex.globalMatch(content).hasNext())
            {
                content = content.replace(regex, "<font color='\\1'>\\2</font>");
            }
        }
        std::cout << content.toStdString() << std::endl;
        return content;
    }
}
