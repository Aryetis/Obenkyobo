#include "Src/mainwindow.h"
#include "ui_mainwindow.h"
#include "Src/fntsetting.h"
#include "Src/qcmexercice.h"
#include "Src/GetMy.h"
#include "Src/tools.h"
#include "Src/symbolsetting.h"
#include <QKeyEvent>
#include "koboplatformfunctions.h"
#include "qsymbolpushbutton.h"

#include <QPainter>

#include <QIcon>
#include <QStyle>

#define POWERBUTTON KoboKey::Key_Power
#define SLEEPCOVERBUTTON KoboKey::Key_SleepCover

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    timer(this), wasBatteryLvl(-1), wasBatteryDisplayFormat(-1), wasBatteryCharging(-1)
{
    std::cout << "LOG: MainWindow::MainWindow()::Start" << std::endl;
    ui->setupUi(this);

    // Setup statusBar
    statusBar = new QMenuBar(parent);
    statusBar->setObjectName("statusBar");
    ui->menuBar->setCornerWidget(statusBar);
    timeDisplay = new QAction("64:39", statusBar);
    statusBar->addAction(timeDisplay);
    actionBatteryIcon = new QAction(QIcon(), "", statusBar);
    statusBar->addAction(actionBatteryIcon);
    actionBatteryTxt = new QAction("init", statusBar);
    statusBar->addAction(actionBatteryTxt);
    float onePercentHeightPx = GetMy::Instance().Descriptor().height/100.0f; // 12.64
    ui->menuBar->setStyleSheet
    (
        QString(
            "QMenuBar { spacing : %1px; }"
            "QMenuBar::item { padding-top : %2px; padding-bottom : %3px; color: black; }"
            "QMenuBar::item#statusBar { background : transparent; }"

            "QMenu::separator { height : 2px ; background : darkgray }"
                ).arg(onePercentHeightPx*2).arg(onePercentHeightPx).arg(onePercentHeightPx/2)
    );

    // Handle time and battery
    connect(&timer, &QTimer::timeout, this, &MainWindow::refreshTimeAndBattery);
    timer.start(1000);
    refreshTimeAndBattery();

    GetMy::Instance().SetMainWindowWidget(this);
    std::cout << "LOG: MainWindow::MainWindow()::End" << std::endl;
}

MainWindow::~MainWindow()
{
    delete timeDisplay;
    delete actionBatteryIcon;
    delete actionBatteryTxt;
    delete statusBar;
    delete ui;
}

// TODO handle sleep (irl too, ahahahah ...)
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == POWERBUTTON || event->key() == SLEEPCOVERBUTTON)
    {
        if (Tools::GetInstance().Sleeping())
            Tools::GetInstance().WakeUp();
        else
            Tools::GetInstance().Sleep();
    }
}

void MainWindow::refreshTimeAndBattery()
{
    bool ugly = false;

    // Handle Time
    QTime time = QTime::currentTime();
    QString text = time.toString((GetMy::Instance().AppSettingWidget().GetDateFormatIdx() == 0) ? "hh:mm" : "hh:mm a" );
    if (text.compare(timeDisplay->text()) != 0)
    {
        timeDisplay->setText(text);
        ugly = true;
    }

    // Handle Battery
    int batteryDisplayFormat = GetMy::Instance().AppSettingWidget().GetBatteryFormatIdx();
    bool isBatteryIconVisible = (batteryDisplayFormat == 0 || batteryDisplayFormat == 1) ? true : false;
    bool isBatteryTextVisible = (batteryDisplayFormat == 0 || batteryDisplayFormat == 2) ? true : false;
    bool isBatteryCharging = KoboPlatformFunctions::isBatteryCharging();
    int batteryLvl = KoboPlatformFunctions::getBatteryLevel();

    // TODO : refactor this using MainWindow::ToolButtonStyle --"
    if (isBatteryCharging != wasBatteryCharging || batteryLvl != wasBatteryLvl || batteryDisplayFormat != wasBatteryDisplayFormat)
    {
        if (isBatteryTextVisible)
            actionBatteryTxt->setText((isBatteryCharging)
                                     ? QString("⚡%1%⚡ ").arg(batteryLvl)
                                     : QString("%1% ").arg(batteryLvl)); // defeated for now, adding an extra space for margin... I hate stylesheet

        if (isBatteryIconVisible)
        {
            if (isBatteryCharging)
                actionBatteryIcon->setIcon(QIcon(":/pictures/Battery/batteryCharging.png"));
            else if (batteryLvl > 77.5)
                actionBatteryIcon->setIcon(QIcon(":/pictures/Battery/battery4.png"));
            else if(batteryLvl > 55)
                actionBatteryIcon->setIcon(QIcon(":/pictures/Battery/battery3.png"));
            else if(batteryLvl > 32.5)
                actionBatteryIcon->setIcon(QIcon(":/pictures/Battery/battery2.png"));
            else if(batteryLvl > 10)
                actionBatteryIcon->setIcon(QIcon(":/pictures/Battery/battery1.png"));
            else
                actionBatteryIcon->setIcon(QIcon(":/pictures/Battery/batteryEmpty.png"));
        }

        ugly = true;
    }

    if (batteryDisplayFormat != wasBatteryDisplayFormat)
    {
        if (!isBatteryIconVisible)
            actionBatteryIcon->setIcon(QIcon());
        if (!isBatteryTextVisible)
            actionBatteryTxt->setText("");

        ugly = true;
    }

    // Resize StatusBar (ugly hack)
    if ( ugly )
        UpdateStatusBarGeometry();

    // Updating status
    wasBatteryDisplayFormat = batteryDisplayFormat;
    wasBatteryCharging = isBatteryCharging;
    wasBatteryLvl = batteryLvl;
}

// TODO : find a proper way to resize statusBar upon size modification
void MainWindow::UpdateStatusBarGeometry()
{
    statusBar->setVisible(false);
    statusBar->setVisible(true);
}

void MainWindow::SwitchStackedWidgetIndex(int i)
{
    std::cout << "LOG: MainWindow::SwitchStackedWidgetIndex(" << i << ")" << std::endl;
    ui->ContentStackedWidget->setCurrentIndex(i);
}

//===========================================================================
void MainWindow::on_actionAbout_triggered()
{
    std::cout << "LOG: MainWindow::on_actionAbout_triggered()" << std::endl;
    ui->ContentStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionExit_triggered()
{
    std::cout << "LOG: MainWindow::on_actionExit_triggered()" << std::endl;
    QApplication::quit();
}



//===========================================================================
void MainWindow::on_actionHiragana_to_Romanji_QCM_triggered()
{
    std::cout << "LOG: MainWindow::on_actionHiragana_to_Romanji_QCM_triggered()" << std::endl;
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Hiragana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set");
}

void MainWindow::on_actionRomanji_to_Hiragana_QCM_triggered()
{
    std::cout << "LOG: MainWindow::on_actionRomanji_to_Hiragana_QCM_triggered()" << std::endl;
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Hiragana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set");
}

void MainWindow::on_actionHiragana_to_Romanji_Kbd_triggered()
{
    std::cout << "LOG: MainWindow::on_actionHiragana_to_Romanji_Kbd_triggered()" << std::endl;
    ui->ContentStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionEdit_Hiragana_Set_triggered()
{
    std::cout << "LOG: MainWindow::on_actionEdit_Hiragana_Set_triggered()" << std::endl;
    QSymbolPushButton::RefreshJpFixedSizedFnt();
    GetMy::Instance().SymbolSettingWidget().InitializeSymbolSetting(SymbolFamilyEnum::hiragana);
    ui->ContentStackedWidget->setCurrentIndex(3);
}



//===========================================================================
void MainWindow::on_actionKatakana_to_Romanji_QCM_triggered()
{
    std::cout << "LOG: MainWindow::on_actionKatakana_to_Romanji_QCM_triggered()" << std::endl;
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Katakana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set");
}

void MainWindow::on_actionRomanji_to_Katakana_QCM_triggered()
{
    std::cout << "LOG: MainWindow::on_actionRomanji_to_Katakana_QCM_triggered()" << std::endl;
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Katakana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set");
}

void MainWindow::on_actionKatakana_to_Romanji_Kbd_triggered()
{
    std::cout << "LOG: MainWindow::on_actionKatakana_to_Romanji_Kbd_triggered()" << std::endl;
    ui->ContentStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionEdit_Katakana_Set_triggered()
{
    std::cout << "LOG: MainWindow::on_actionEdit_Katakana_Set_triggered()" << std::endl;
    QSymbolPushButton::RefreshJpFixedSizedFnt();
    GetMy::Instance().SymbolSettingWidget().InitializeSymbolSetting(SymbolFamilyEnum::katakana);
    ui->ContentStackedWidget->setCurrentIndex(3);
}



//===========================================================================
void MainWindow::on_actionApplication_Setting_triggered()
{
    std::cout << "LOG: MainWindow::on_actionApplication_Setting_triggered()" << std::endl;
    ui->ContentStackedWidget->setCurrentIndex(4);
}

void MainWindow::on_actionFonts_Setting_triggered()
{
    std::cout << "LOG: MainWindow::on_actionFonts_Setting_triggered()" << std::endl;
    ui->ContentStackedWidget->setCurrentIndex(5);
}

void MainWindow::on_actionScreen_Setting_triggered()
{
    std::cout << "LOG: MainWindow::on_actionScreen_Setting_triggered()" << std::endl;
    if (GetMy::Instance().ScreenSettingsWidget().AreSettingsAvailablePopup())
        ui->ContentStackedWidget->setCurrentIndex(6);
}



//===========================================================================
void MainWindow::on_actionKana_To_Romanji_MCQ_triggered()
{

}

void MainWindow::on_actionRomanji_to_Kanas_MCQ_triggered()
{

}

void MainWindow::on_actionLearn_Edit_Set_triggered()
{
    std::cout << "LOG: MainWindow::on_actionLearn_Edit_Set_triggered()" << std::endl;
    GetMy::Instance().VocabularyLearnEditSetWidget()->InitializeVocabularyLearnEditSet();
    ui->ContentStackedWidget->setCurrentIndex(7);
}


//===========================================================================
