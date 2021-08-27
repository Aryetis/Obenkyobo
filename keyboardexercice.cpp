#include "keyboardexercice.h"
#include "ui_keyboardexercice.h"

KeyboardExercice::KeyboardExercice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyboardExercice)
{
    ui->setupUi(this);
}

KeyboardExercice::~KeyboardExercice()
{
    delete ui;
}
