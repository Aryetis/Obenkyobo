#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fntsetting.h"
#include "qcmexercice.h"
#include "GetMy.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Register shortcut because QtWidget hates singleton
    GetMy::GetInstance().SetFntSettingWidget(static_cast<FntSetting*>(ui->ContentStackedWidget->widget(5)));

    FntSetting& fntSettings = GetMy::GetInstance().FntSettingWidget();
    // Registering Hiragana fonts
    fntSettings.RegisterHiraganaFont(":/HiraganaFonts/PJ_Hiragana.ttf");
    fntSettings.RegisterHiraganaFont(":/HiraganaFonts/DotGothic16-Regular.ttf");

    // Registering Katakana fonts
    fntSettings.RegisterKatakanaFont(":/HiraganaFonts/DotGothic16-Regular.ttf");

    // Registering Romanji fonts
    fntSettings.RegisterRomanjiFont(":/HiraganaFonts/DotGothic16-Regular.ttf");

    fntSettings.InitializeFntSetting();
    // Set Romanji font across the whole application (probably not the best idea)
//    QFont font(FntSetting::GetCurrentRomanjiFnt());
//    font.setStyleHint(QFont::Monospace);
//    QApplication::setFont(font);
}

MainWindow::~MainWindow()
{
    delete ui;
}



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
    GetMy::GetInstance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM);
    ui->ContentStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionRomanji_to_Hiragana_QCM_triggered()
{
    GetMy::GetInstance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM);
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
    GetMy::GetInstance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM);
    ui->ContentStackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionRomanji_to_Katakana_QCM_triggered()
{
    GetMy::GetInstance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM);
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
