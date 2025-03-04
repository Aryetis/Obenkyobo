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
            // Goal, transform this : <red> BLAH </red>||</>
            // into this : <font color='red'> BLAH </font>
            // Also support multi lines and nested ones
            QRegularExpressionMatchIterator regexMatchIterator {colorSyntaxRegex.globalMatch(content)};
            int offset=0; // difference builds up as we replace text
            while(regexMatchIterator.hasNext())
            {
                QRegularExpressionMatch match = regexMatchIterator.next();

                // closing color sequence (handle both </> and </red> but not </span>)
                if (QString firstMatch{match.captured(1)}; firstMatch == "/"
                    || ( firstMatch[0] == '/' && QColor::colorNames().contains(QStringRef(&firstMatch, 1, match.captured(1).length()-1))))
                {
                    QString replaceBy {"</font>"};
                    // Can't use QString::replace(int i, int n, QString), cause it can go out of expected range and overwrite stuff
                    content = content.remove(match.capturedStart()+offset, match.capturedLength());
                    content = content.insert(match.capturedStart()+offset, replaceBy);
                    offset += replaceBy.length()-match.captured(0).length();
                }

                // opening color sequence
                // check that captured color match with SVG Color names spec
                // https://www.w3.org/TR/SVG11/types.html#ColorKeywords
                else if(QColor::colorNames().contains(match.captured(1)))
                {
                    QString replaceBy {"<font color='"+match.captured(1)+"'>"};
                    content = content.remove(match.capturedStart()+offset, match.capturedLength());
                    content = content.insert(match.capturedStart()+offset, replaceBy);
                    offset += replaceBy.length()-match.captured(0).length();
                }
            }
        }

        //std::cout << content.toStdString() << std::endl;
        return content;
    }
}
