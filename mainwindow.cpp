#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fntsetting.h"
#include "qcmexercice.h"
#include "GetMy.h"
#include "tools.h"
#include "symbolsetting.h"
#include <QKeyEvent>
#include "koboplatformfunctions.h"

#include <QPainter>

#define POWERBUTTON KoboKey::Key_Power
#define SLEEPCOVERBUTTON KoboKey::Key_SleepCover

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    statusBar(ui->menuBar), timeDisplay("20:42", &statusBar),
    actionBatteryIcon(QIcon(":/pictures/Battery/battery1.png"), "batteryIcon", &statusBar),
    actionBatteryTxt("100%", &statusBar),
    timer(this), wasBatteryLvl(-1)
{
    ui->setupUi(this);

    // Setup statusBar
    ui->menuBar->setCornerWidget(&statusBar);
    statusBar.addAction(&timeDisplay);
    statusBar.addAction(&actionBatteryIcon);
    statusBar.addAction(&actionBatteryTxt);
    statusBar.setStyleSheet("QMenuBar::item { color: black; background: transparent; }");

    // Handle time and battery
    connect(&timer, &QTimer::timeout, this, &MainWindow::refreshTimeAndBattery);
    timer.start(1000);
    actionBatteryIcon.setIconText("init");
    refreshTimeAndBattery();
}

MainWindow::~MainWindow()
{
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

bool MainWindow::IsLocalTimeFormatUS()
{
    // TODO handle Settings override
    QFile inputFile("/mnt/onboard/.kobo/Kobo/Kobo eReader.conf");
    inputFile.open(QIODevice::ReadOnly);
    if (!inputFile.isOpen())
        return true;

    QTextStream stream(&inputFile);
    for (QString line = stream.readLine(); !line.isNull(); line = stream.readLine())
    {
        QStringList nombres = line.split("=", Qt::SkipEmptyParts);
        if (nombres.size() == 2)
        {
            QString prefix = nombres[0];
            QString value = nombres[1];
            if (prefix.compare("CurrentLocale") == 0)
            {
                if (value.size()>3)
                {
                    value = nombres[1].mid(0,3);
                    return (value.compare("en_") == 0) ? true : false;
                }
                return false;
            }
        }
    }

    return true;
}

void MainWindow::refreshTimeAndBattery()
{
    bool ugly = false;

    // Handle Time
    QTime time = QTime::currentTime();
    QString text = time.toString((IsLocalTimeFormatUS()) ? "hh:mm a" : "hh:mm" );
    if (text.compare(timeDisplay.text()) != 0)
    {
        timeDisplay.setText(text);
        ugly = true;
    }

    // Handle Battery
    int batteryLvl = KoboPlatformFunctions::getBatteryLevel();
    if (KoboPlatformFunctions::isBatteryCharging())
    {
        if (actionBatteryIcon.iconText() != "lvlCharging" || actionBatteryIcon.iconText() == "init")
        {
            actionBatteryIcon.setIcon(QIcon(":/pictures/Battery/batteryCharging.png"));
            actionBatteryTxt.setText(QString("⚡%1%⚡").arg(batteryLvl));
            actionBatteryIcon.setIconText("lvlCharging");
            ugly = true;
        }
    }
    else
    {
        if (wasBatteryLvl != batteryLvl || actionBatteryIcon.iconText() == "lvlCharging"
                                        || actionBatteryIcon.iconText() == "init")
        {
            actionBatteryTxt.setText(QString("%1%").arg(batteryLvl));
            wasBatteryLvl = batteryLvl;

            if (batteryLvl > 80 && actionBatteryIcon.iconText() != "lvl4")
            {
                actionBatteryIcon.setIcon(QIcon(":/pictures/Battery/battery4.png"));
                actionBatteryIcon.setIconText("lvl4");
            }
            else if(batteryLvl > 60 && actionBatteryIcon.iconText() != "lvl3")
            {
                actionBatteryIcon.setIcon(QIcon(":/pictures/Battery/battery3.png"));
                actionBatteryIcon.setIconText("lvl3");
            }
            else if(batteryLvl > 40 && actionBatteryIcon.iconText() != "lvl2")
            {
                actionBatteryIcon.setIcon(QIcon(":/pictures/Battery/battery2.png"));
                actionBatteryIcon.setIconText("lvl2");
            }
            else if(batteryLvl > 20 && actionBatteryIcon.iconText() != "lvl1")
            {
                actionBatteryIcon.setIcon(QIcon(":/pictures/Battery/battery1.png"));
                actionBatteryIcon.setIconText("lvl1");
            }
            else if(actionBatteryIcon.iconText() != "lvlEmpty")
            {
                actionBatteryIcon.setIcon(QIcon(":/pictures/Battery/batteryEmpty.png"));
                actionBatteryIcon.setIconText("lvlEmpty");
            }

            ugly = true;
        }
    }

    // Resize StatusBar
    if ( ugly ) // TODO : find a proper way to resize statusBar upon size modification
    {
        statusBar.setVisible(false);
        statusBar.setVisible(true);
    }
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
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Hiragana_to_Romanji_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Hiragana, please enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at : Main->Hiragana->Edit Hiragana Set");
}

void MainWindow::on_actionRomanji_to_Hiragana_QCM_triggered()
{
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Hiragana, please enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at : Main->Hiragana->Edit Hiragana Set");
}

void MainWindow::on_actionHiragana_to_Romanji_Kbd_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionEdit_Hiragana_Set_triggered()
{
    GetMy::Instance().SymbolSettingWidget().InitializeSymbolSetting(SymbolFamilyEnum::hiragana);
    ui->ContentStackedWidget->setCurrentIndex(3);
}



//===========================================================================

void MainWindow::on_actionKatakana_to_Romanji_QCM_triggered()
{
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Katakana, please enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at : Main->Hiragana->Edit Hiragana Set");
}

void MainWindow::on_actionRomanji_to_Katakana_QCM_triggered()
{
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Katakana, please enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at : Main->Hiragana->Edit Hiragana Set");
}

void MainWindow::on_actionKatakana_to_Romanji_Kbd_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionEdit_Katakana_Set_triggered()
{
    GetMy::Instance().SymbolSettingWidget().InitializeSymbolSetting(SymbolFamilyEnum::katakana);
    ui->ContentStackedWidget->setCurrentIndex(3);
}



//===========================================================================
void MainWindow::on_actionApplication_Setting_triggered()
{
    GetMy::Instance(). AppSettingWidget().InitializeUIValues();
    ui->ContentStackedWidget->setCurrentIndex(4);
}

void MainWindow::on_actionFonts_Setting_triggered()
{
    ui->ContentStackedWidget->setCurrentIndex(5);
}

void MainWindow::on_actionScreen_Setting_triggered()
{
    if (GetMy::Instance().ScreenSettingsWidget().AreSettingsAvailablePopup())
        ui->ContentStackedWidget->setCurrentIndex(6);
}
