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
            //************** ADDITIONAL COLOR MARKDOWN SHORTCUT ***************
            // transform this : <red> BLAH </red>||</>
            // into this : <font color="red"> BLAH </font>
            // Also support multi lines and nested ones
            // captured colors match with SVG Color names spec
            // https://www.w3.org/TR/SVG11/types.html#ColorKeywords
            // LIMITATION 1 : Everything in between <color> tags must be cleansed from \n \r
            //                otherwise it can mess up the margin-left propery and even change display order /
            //                push it further down the document... Thanks qt
            //                Or.... we can manually introduce <font color="red" at the start of every new line
            //                included in between tags... Let's try that...
            // LIMITATION 2 : If you take the following text :
            //                    <red>
            //                    line 1
            //                    </>
            //                It will introcuce an empty <font color='red'></font>, creating a space before "line 1"
            //                => let's prevent the insertion of empty <font></font> insertion
            //                   (only) when detecting an "open sequence"/<color>. Keep in mind that chaining two+ real empty lines,
            //                   is markdown new paragraph syntax.
            // BONUS : "Yes but what about old mac return characters line break ? shouldn't you use "(\r\n|\r|\n) ?"
            //         Can't test, don't care... Get some computer from this century.
            //*****************************************************************
            static const QRegularExpression colorSyntaxRegex {"(?:<([\/a-zA-Z]*)>)(\r?\n)?|(\r?\n)"}; // reminder, (?:azeaze) non capturing group
            QRegularExpressionMatchIterator regexMatchIterator {colorSyntaxRegex.globalMatch(content)};
            int offset=0; // difference builds up as we replace text
            QQueue<QString> colorStack;
            while(regexMatchIterator.hasNext())
            {
                QRegularExpressionMatch match = regexMatchIterator.next();
                QString replaceBy{""};

                /******** opening color sequence ********/
                if(QColor::colorNames().contains(match.captured(1)))
                {
                    colorStack.enqueue(match.captured(1));
                    if (QChar nextChar = content.at(match.capturedEnd()+offset); nextChar == '\n' || nextChar == '\r')
                    {
                        std::cout << "JACKPOT" << std::endl;
                        content = content.remove(match.capturedStart()+offset, match.capturedLength());
                        offset -= match.captured().length();
                    }
                    else
                        replaceBy = "<font color='"+match.captured(1)+"'>";
                }
                /******** closing color sequence (handle both </> and </red> but not </span>) ********/
                else if (QString firstMatch{match.captured(1)}; firstMatch == "/"
                    || ( firstMatch[0] == '/' && QColor::colorNames().contains(QStringRef(&firstMatch, 1, match.captured(1).length()-1))))
                {
                    if (colorStack.empty())
                        std::cerr << "ERROR : <color> markdown shortcut malformed @" << match.capturedStart()+offset << std::endl;
                    else
                        colorStack.dequeue();

                    // Don't eat the line return (if there is any) => add it back
                    replaceBy = "</font>"+match.captured(2);
                }
                /******** Repeating <font> tag for each new line ********/
                else if(!colorStack.empty() && !match.captured(3).isEmpty())
                    replaceBy = "</font>\n<font color='"+colorStack.head()+"'>";

                /******** Let's actually replace things now ********/
                if (replaceBy != "")
                {
                    // Can't use QString::replace(int i, int n, QString), cause it can go out of expected range and overwrite stuff
                    content = content.remove(match.capturedStart()+offset,match.capturedLength());
                    content = content.insert(match.capturedStart()+offset, replaceBy);
                    offset += replaceBy.length()-match.captured(0).length();
                }
            }
        }

        std::cout << content.toStdString() << std::endl;
        return content;
    }
}
