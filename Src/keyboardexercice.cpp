#include "Src/keyboardexercice.h"
#include "ui_keyboardexercice.h"

#include <iostream>

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
