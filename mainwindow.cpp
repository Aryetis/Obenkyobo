#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFontDatabase"
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(":/fonts/fonts/HIRAGANA.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont hiragana(family);
//    ui->mainLabelTest->setFont(hiragana);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

//void MainWindow::on_pushButton_clicked()
//{
//    int foo = std::atoi(ui->button1Label->text().toStdString().c_str()) +1;
//    ui->button1Label->setText(std::to_string(foo).c_str());
//}

void MainWindow::on_actionHiragana_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionKatakana_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionAbout_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(0);
}
