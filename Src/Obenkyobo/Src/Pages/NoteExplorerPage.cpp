#include "Src/Pages/NoteExplorerPage.h"
#include <ui_NoteExplorerPage.h>

NoteExplorerPage::NoteExplorerPage(QWidget *parent) :
    QWidget(parent), ui(new Ui::NoteExplorerPage)
{
    ui->setupUi(this);
}

NoteExplorerPage::~NoteExplorerPage()
{
    delete ui;
}
