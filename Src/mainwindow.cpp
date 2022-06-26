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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    timer(this), wasBatteryLvl(-1), wasBatteryDisplayFormat(-1), wasBatteryCharging(-1)
{
    std::cout << "LOG: MainWindow::MainWindow()::Start" << std::endl;
    ui->setupUi(this);

    // Dithering generate artifcats on certain screens, not worth it
    KoboPlatformFunctions::enableDithering(false, false);

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

    // Refresh button
    refresh = new QAction(QIcon(":/pictures/Logos/refresh64.png"), "ref", ui->menuBar);
    refresh->setObjectName("refresh");
    ui->menuBar->addAction(refresh);
    connect(refresh, &QAction::triggered, this, &MainWindow::on_refresh_triggered);
    connect(refresh, &QAction::hovered, this, &MainWindow::on_refresh_hovered);

    // Stylesheet
    float onePercentHeightPx = GetMy::Instance().Descriptor().height/100.0f; // 12.64
    ui->menuBar->setStyleSheet
    (
        QString(
            "QMenuBar { spacing : %1px; }"
            "QMenuBar::item { padding-top : %2px; padding-bottom : %3px; color: black; }"
            "QMenuBar::item { padding-top : %2px; padding-bottom : %3px; color: black; }"
            "QMenu::item#menuHiragana { padding : %4px; }"

            // hack : margin-right and left, to account for shifting right-arrow "correctly"
            "QMenu::right-arrow#menuMain { image: url(:/pictures/Arrows/qmenuArrowRight.png); left: %5px;}"
            "QMenu::item#menuMain { padding : %4px; margin-right : %5px; }"

            "QMenuBar::item#statusBar { background : transparent; }"

            "QMenu::separator { height : 2px ; background : darkgray }"
                ).arg(onePercentHeightPx*2).arg(onePercentHeightPx*2).arg(onePercentHeightPx).arg(onePercentHeightPx/2).arg(ui->menuHiragana->height())
    );

    // Handle time and battery
    connect(&timer, &QTimer::timeout, this, &MainWindow::refreshTimeAndBattery);
    timer.start(1000);
    refreshTimeAndBattery();

    GetMy::Instance().SetMainWindowWidget(this);
    std::cout << "LOG: MainWindow::MainWindow()::End" << std::endl;

    DisplayFirstTimeMainWindowPagePopup();
}

MainWindow::~MainWindow()
{
    delete timeDisplay;
    delete actionBatteryIcon;
    delete actionBatteryTxt;
    delete statusBar;
    delete refresh;
    delete ui;
}

void MainWindow::AggressiveClearScreen(bool force /*=false*/) const // requires adding DRAW=1 to fbink build parameter of koboplatformplugin
{
    if (GetMy::Instance().AppSettingWidget().GetKanaHardRefresh() || force)
    {
        KoboPlatformFunctions::setFullScreenRefreshMode(WaveForm::WaveForm_GC16);
        KoboPlatformFunctions::clearScreen(true);
        ui->centralWidget->repaint();
        ui->menuBar->repaint();
    }
}

// TODO handle sleep (irl too, ahahahah ...)
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == KoboKey::Key_Power)
    {
        if (Tools::GetInstance().Sleeping())
            Tools::GetInstance().WakeUp();
        else
            Tools::GetInstance().Sleep();
    }
    else if (event->key() == KoboKey::Key_SleepCover && !Tools::GetInstance().Sleeping())
        Tools::GetInstance().Sleep();
    else if (event->key() == KoboKey::Key_Light)
        GetMy::Instance().ScreenSettingsWidget().ToggleLight();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) // requires qpa fix https://github.com/Rain92/qt5-kobo-platform-plugin/pull/5
{
    if (event->key() == KoboKey::Key_SleepCover && Tools::GetInstance().Sleeping())
        Tools::GetInstance().WakeUp();
}

void MainWindow::DisplayFirstTimeMainWindowPagePopup()
{
    if ( GetMy::Instance().AppSettingWidget().GetSettingsSerializer()->value("AppSettings/firstTimeMainWindowPage", true).toBool() )
    {
        Tools::GetInstance().DisplayPopup("This software is still in development, Some functions are not implemented yet.\n"
                                          "Sleep functionality is not supported for now.\n"
                                          "Therefore it is recommended that you close Obenkyobo between each session to save battery life.\n"
                                          "Each popup will be used only once and sparingly to introduce some mechanisms.\n"
                                          "If you wish to see any popup again later on, please go to Settings->Application->Reset Help Popup\n\n"
                                          "Thanks for reading, enjoy."
                                          , 0.75f);
        GetMy::Instance().AppSettingWidget().GetSettingsSerializer()->setValue("AppSettings/firstTimeMainWindowPage", false);
    }
}

void MainWindow::DisplayFirstTimeKanasEditPagePopup()
{
    if ( GetMy::Instance().AppSettingWidget().GetSettingsSerializer()->value("AppSettings/firstTimeKanasEditPage", true).toBool() )
    {
        Tools::GetInstance().DisplayPopup("Here you can add/remove kanas to the related MCQ's guesses.\n"
                                          "The background color and circle gyzmo indicate their status.\n"
                                          "The number at the bottom right corner indicate its Learning Score (LS).\n"
                                          "It ranges from 0 to 5 and reflets your progress.\n"
                                          "You can use the checkboxes to quickly toggle kanas from associated section."
                                         , 0.5f);
        GetMy::Instance().AppSettingWidget().GetSettingsSerializer()->setValue("AppSettings/firstTimeKanasEditPage", false);
    }
}

void MainWindow::OnSleep() const
{
    if (Tools::GetInstance().Sleeping())
        return;

    disconnect(&timer, nullptr, nullptr, nullptr);
    actionBatteryIcon->setIcon(QIcon());
    actionBatteryTxt->setText("");
    timeDisplay->setText("ZZZzzzz... ");

    UpdateStatusBarGeometry();
}

void MainWindow::OnWakeUp()
{
    if (!Tools::GetInstance().Sleeping())
        return;

    connect(&timer, &QTimer::timeout, this, &MainWindow::refreshTimeAndBattery);
    // reinitializing icons format
    wasBatteryLvl = -1;
    wasBatteryDisplayFormat = -1;
    wasBatteryCharging = -1;

    UpdateStatusBarGeometry();
}

void MainWindow::on_refresh_triggered()
{
    AggressiveClearScreen(true);
    on_refresh_hovered();
}

void MainWindow::on_refresh_hovered()
{
    // hack : I DO NOT want any stylesheet when triggering, hovering from another menu, anything !
    // but obviously qt doesn't seem to allow us to stylesheet for one precise QMenuBar::item
    // Soooooo there we go... I embrace the jank
    refresh->setEnabled(false);
    refresh->setEnabled(true);
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
            actionBatteryTxt->setText(" "); // ugly hack to get a right-margin because QMenuBar qss is acting weird on my kobos.

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
void MainWindow::UpdateStatusBarGeometry() const
{
    statusBar->setVisible(false);
    statusBar->setVisible(true);
}

void MainWindow::SwitchStackedWidgetIndex(int i)
{
    std::cout << "LOG: MainWindow::SwitchStackedWidgetIndex(" << i << ")" << std::endl;
    ui->ContentStackedWidget->setCurrentIndex(i);
    AggressiveClearScreen();
}

//===========================================================================
void MainWindow::on_actionAbout_triggered()
{
    std::cout << "LOG: MainWindow::on_actionAbout_triggered()" << std::endl;
    ui->ContentStackedWidget->setCurrentIndex(0);
    DisplayFirstTimeMainWindowPagePopup();
    AggressiveClearScreen();
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
        AggressiveClearScreen();
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Hiragana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set"
                , 0.3f);
}

void MainWindow::on_actionRomanji_to_Hiragana_QCM_triggered()
{
    std::cout << "LOG: MainWindow::on_actionRomanji_to_Hiragana_QCM_triggered()" << std::endl;
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Romanji_to_Hiragana_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
        AggressiveClearScreen();
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Hiragana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set"
                , 0.3f);
}

void MainWindow::on_actionHiragana_to_Romanji_Kbd_triggered()
{
//    std::cout << "LOG: MainWindow::on_actionHiragana_to_Romanji_Kbd_triggered()" << std::endl;
//    ui->ContentStackedWidget->setCurrentIndex(2);
//    AggressiveClearScreen();
}

void MainWindow::on_actionEdit_Hiragana_Set_triggered()
{
    std::cout << "LOG: MainWindow::on_actionEdit_Hiragana_Set_triggered()" << std::endl;
    QSymbolPushButton::RefreshJpFixedSizedFnt();
    GetMy::Instance().SymbolSettingWidget().InitializeSymbolSetting(SymbolFamilyEnum::hiragana);
    ui->ContentStackedWidget->setCurrentIndex(3);
    DisplayFirstTimeKanasEditPagePopup();
    AggressiveClearScreen();
}



//===========================================================================
void MainWindow::on_actionKatakana_to_Romanji_QCM_triggered()
{
    std::cout << "LOG: MainWindow::on_actionKatakana_to_Romanji_QCM_triggered()" << std::endl;
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Katakana_to_Romanji_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
        AggressiveClearScreen();
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Katakana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set"
                , 0.3f);
}

void MainWindow::on_actionRomanji_to_Katakana_QCM_triggered()
{
    std::cout << "LOG: MainWindow::on_actionRomanji_to_Katakana_QCM_triggered()" << std::endl;
    if (GetMy::Instance().AppSettingWidget().IsThereEnough(QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM))
    {
        GetMy::Instance().QcmExerciceWidget().InitializeExercice(QcmExercice::QcmExerciceType::Romanji_to_Katakana_QCM, true);
        ui->ContentStackedWidget->setCurrentIndex(1);
        AggressiveClearScreen();
    }
    else
        Tools::GetInstance().DisplayPopup(
                "Not enough enabled Katakana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingWidget().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set"
                , 0.3f);
}

void MainWindow::on_actionKatakana_to_Romanji_Kbd_triggered()
{
//    std::cout << "LOG: MainWindow::on_actionKatakana_to_Romanji_Kbd_triggered()" << std::endl;
//    ui->ContentStackedWidget->setCurrentIndex(2);
//    AggressiveClearScreen();
}

void MainWindow::on_actionEdit_Katakana_Set_triggered()
{
    std::cout << "LOG: MainWindow::on_actionEdit_Katakana_Set_triggered()" << std::endl;
    QSymbolPushButton::RefreshJpFixedSizedFnt();
    GetMy::Instance().SymbolSettingWidget().InitializeSymbolSetting(SymbolFamilyEnum::katakana);
    ui->ContentStackedWidget->setCurrentIndex(3);
    DisplayFirstTimeKanasEditPagePopup();
    AggressiveClearScreen();
}



//===========================================================================
void MainWindow::on_actionApplication_Setting_triggered()
{
    std::cout << "LOG: MainWindow::on_actionApplication_Setting_triggered()" << std::endl;
    GetMy::Instance().AppSettingWidget().UpdateScoreCounters();
    ui->ContentStackedWidget->setCurrentIndex(4);
    AggressiveClearScreen();
}

void MainWindow::on_actionFonts_Setting_triggered()
{
    std::cout << "LOG: MainWindow::on_actionFonts_Setting_triggered()" << std::endl;
    ui->ContentStackedWidget->setCurrentIndex(5);
    AggressiveClearScreen();
}

void MainWindow::on_actionScreen_Setting_triggered()
{
    std::cout << "LOG: MainWindow::on_actionScreen_Setting_triggered()" << std::endl;
    if (GetMy::Instance().ScreenSettingsWidget().AreSettingsAvailablePopup())
    {
        ui->ContentStackedWidget->setCurrentIndex(6);
        AggressiveClearScreen();
    }
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
    AggressiveClearScreen();

    if ( GetMy::Instance().AppSettingWidget().GetSettingsSerializer()->value("AppSettings/firstTimeVocabListPage", true).toBool() )
    {
        Tools::GetInstance().DisplayPopup("Here is displayed every \"vocab sheet\" you have, check Obenkyobo's github page for more information about them.\n"
                                          "Click on any one to learn/display its content.\n"
                                          "Ignore the LS and checkbox for now, those are related unimplemented features for now."
                                          , 0.4f);
        GetMy::Instance().AppSettingWidget().GetSettingsSerializer()->setValue("AppSettings/firstTimeVocabListPage", false);
    }
}


//===========================================================================
