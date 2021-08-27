#include "qcmexercice.h"
#include "ui_qcmexercice.h"

QcmExercice::QcmExercice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QcmExercice)
{
    ui->setupUi(this);
}

QcmExercice::~QcmExercice()
{
    delete ui;
}
