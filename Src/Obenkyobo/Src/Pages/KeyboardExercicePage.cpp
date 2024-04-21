#include "Src/Pages/KeyboardExercicePage.h"
#include "ui_KeyboardExercicePage.h"

KeyboardExercicePage::KeyboardExercicePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyboardExercicePage)
{
    ui->setupUi(this);
}

KeyboardExercicePage::~KeyboardExercicePage()
{
    delete ui;
}
