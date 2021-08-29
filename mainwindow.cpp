#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFontDatabase"
#include <iostream>
#include <string>
#include "qcmexercice.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(":/fonts/fonts/HIRAGANA.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont hiragana(family);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void on_actionHiragana_to_Romanji_QCM_triggered();
void on_actionRomanji_to_Hiragana_QCM_triggered();
void on_actionHiragana_to_Romanji_Kbd_triggered();

void on_actionKatakana_to_Romanji_QCM_triggered();
void on_actionRomanji_to_Katakana_QCM_triggered();
void on_actionKatakana_to_Romanji_Kbd_triggered();

void on_actionEdit_Symbols_Set_triggered();
void on_actionApplication_Setting_triggered();


//===========================================================================
void MainWindow::on_actionAbout_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

//===========================================================================
void MainWindow::on_actionHiragana_to_Romanji_QCM_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(1);
    QcmExercice* exercice = static_cast<QcmExercice*>(ui->ContentStackedWidget->currentWidget());
//    ui->ContentStackedWidget->currentWidget()
    if (exercice)
    {
        exercice->InitializeExercice();
    }
}


void MainWindow::on_actionRomanji_to_Hiragana_QCM_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionHiragana_to_Romanji_Kbd_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionEdit_Hiragana_Set_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(3);
}


//===========================================================================

void MainWindow::on_actionKatakana_to_Romanji_QCM_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionRomanji_to_Katakana_QCM_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionKatakana_to_Romanji_Kbd_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionEdit_Katakana_Set_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(3);
}



//===========================================================================
void MainWindow::on_actionApplication_Setting_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(4);
}

void MainWindow::on_actionFonts_Setting_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(5);
}

void MainWindow::on_actionScreen_Setting_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(6);
}
