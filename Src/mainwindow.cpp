#include <QKeyEvent>
#include <QPainter>
#include <QIcon>
#include <QStyle>
#include "Src/mainwindow.h"
#include "ui_mainwindow.h"
#include "Src/Pages/QcmExercicePage.h"
#include "Src/GetMy.h"
#include "Src/Tools.h"
#include "Src/Pages/KanaEditPage.h"
#include "koboplatformfunctions.h"
#include "Src/Widgets/KanaEditButtonWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    timer(this), wasBatteryLvl(-1), wasBatteryDisplayFormat(-1), wasBatteryCharging(-1), curBatteryIconStr("")
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

            // hack : margin-right and left, to account for shifting right-arrow "correctly"
            "QMenu::right-arrow#menuMain { image: url(:/pictures/Arrows/qmenuArrowRight.png); left: %5px;}"
            "QMenu::item#menuMain { margin-right : %5px; }"

            "QMenuBar::item#statusBar { background : transparent; }"

            "QMenu::item { padding : %4px; }"
            "QMenu::separator { height : 2px ; }"
                ).arg(onePercentHeightPx*2).arg(onePercentHeightPx*2).arg(onePercentHeightPx).arg(onePercentHeightPx/2).arg(ui->menuHiragana->height())
    );

    // Handle time and battery
    connect(&timer, &QTimer::timeout, this, &MainWindow::refreshTimeAndBattery);
    timer.start(1000);
    refreshTimeAndBattery();

    GetMy::Instance().SetMainWindowInst(this);
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

void MainWindow::AggressiveClearScreen(bool force /*=false*/) const
{
    if (GetMy::Instance().AppSettingsPageInst().GetKanaHardRefresh() || force)
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
        if (GetMy::Instance().ToolsInst()->GetDeviceState() == DeviceState::asleep)
            GetMy::Instance().ToolsInst()->WakeUp();
        else if (GetMy::Instance().ToolsInst()->GetDeviceState() == DeviceState::awake)
            GetMy::Instance().ToolsInst()->Sleep();
    }
    else if (event->key() == KoboKey::Key_SleepCover && GetMy::Instance().ToolsInst()->GetDeviceState() == DeviceState::awake)
        GetMy::Instance().ToolsInst()->Sleep();
    else if (event->key() == KoboKey::Key_Light)
        GetMy::Instance().ScreenSettingsPageInst().ToggleLight();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == KoboKey::Key_SleepCover && GetMy::Instance().ToolsInst()->GetDeviceState() == DeviceState::asleep)
        GetMy::Instance().ToolsInst()->WakeUp();
}

void MainWindow::DisplayFirstTimeMainWindowPagePopup()
{
    if ( GetMy::Instance().AppSettingsPageInst().GetSettingsSerializer()->value("AppSettings/firstTimeMainWindowPage", true).toBool() )
    {
        GetMy::Instance().ToolsInst()->DisplayPopup("This software is still in development, Some functions are not implemented yet.\n"
                                          "Sleep functionality is not supported for now.\n"
                                          "Therefore it is recommended that you close Obenkyobo between each session to save battery life.\n"
                                          "Each popup will be used only once and sparingly to introduce some mechanisms.\n"
                                          "If you wish to see any popup again later on, please go to Settings->Application->Reset Help Popup\n\n"
                                          "Thanks for reading, enjoy.", true);
        GetMy::Instance().AppSettingsPageInst().GetSettingsSerializer()->setValue("AppSettings/firstTimeMainWindowPage", false);
    }
}

void MainWindow::DisplayFirstTimeKanasEditPagePopup()
{
    if ( GetMy::Instance().AppSettingsPageInst().GetSettingsSerializer()->value("AppSettings/firstTimeKanasEditPage", true).toBool() )
    {
        GetMy::Instance().ToolsInst()->DisplayPopup("Here you can add/remove kanas to the related MCQ's guesses.\n"
                                          "The background color and circle gyzmo indicates their status.\n"
                                          "The number at the bottom right corner indicates its Learning Score (LS).\n"
                                          "It ranges reflects your progress, ranging from 0 to 5.\n"
                                          "You can use the checkboxes to quickly toggle whole section.");
        GetMy::Instance().AppSettingsPageInst().GetSettingsSerializer()->setValue("AppSettings/firstTimeKanasEditPage", false);
    }
}

void MainWindow::OnSleep() const
{
//    if (GetMy::Instance().ToolsInst()->GetDeviceState() != DeviceState::busy)
//        return;

    disconnect(&timer, nullptr, nullptr, nullptr);
    actionBatteryIcon->setIcon(QIcon());
    actionBatteryTxt->setText("");
    timeDisplay->setText("ZZZzzzz... ");

    UpdateStatusBarGeometry();
}

void MainWindow::OnWakeUp()
{
//    if (GetMy::Instance().ToolsInst()->GetDeviceState() != DeviceState::busy)
//        return;

    connect(&timer, &QTimer::timeout, this, &MainWindow::refreshTimeAndBattery);
    // reinitializing icons format
    wasBatteryLvl = -1;
    wasBatteryDisplayFormat = -1;
    wasBatteryCharging = -1;
    curBatteryIconStr = "";
    timeDisplay->setText("Waking up !");

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
    if (GetMy::Instance().ToolsInst()->GetDeviceState() != DeviceState::awake)
        return;

    bool ugly = false;

    // Handle Time
    QTime time = QTime::currentTime();
    QString timeText = time.toString(GetMy::Instance().ToolsInst()->IsLocalTimeFormatUS() ? "hh:mm a" : "hh:mm" );
    if (timeDisplay->text() != timeText)
    {
        timeDisplay->setText(timeText);
        ugly = true;
    }

    // Handle Battery
    int batteryDisplayFormat = GetMy::Instance().AppSettingsPageInst().GetBatteryFormatIdx();
    bool isBatteryIconVisible = (batteryDisplayFormat == 0 || batteryDisplayFormat == 1) ? true : false;
    bool isBatteryTextVisible = (batteryDisplayFormat == 0 || batteryDisplayFormat == 2) ? true : false;
    bool isBatteryCharging = KoboPlatformFunctions::isBatteryCharging();
    int batteryLvl = KoboPlatformFunctions::getBatteryLevel();

    // TODO : refactor this using MainWindow::ToolButtonStyle --"
    if (isBatteryCharging != wasBatteryCharging || batteryLvl != wasBatteryLvl || batteryDisplayFormat != wasBatteryDisplayFormat)
    {
        if (isBatteryTextVisible)
        {
            QString batteryTxt =(isBatteryCharging) ? QString("⚡%1%⚡ ").arg(batteryLvl) : QString("%1% ").arg(batteryLvl);
            if (actionBatteryTxt->text() != batteryTxt)
                actionBatteryTxt->setText(batteryTxt); // defeated for now, adding an extra space for margin... I hate stylesheet
        }

        if (isBatteryIconVisible)
        {
            QString newBatteryIconStr;
            if (isBatteryCharging)
                newBatteryIconStr = ":/pictures/Battery/batteryCharging.png";
            else if (batteryLvl > 77.5)
                newBatteryIconStr = ":/pictures/Battery/battery4.png";
            else if(batteryLvl > 55)
                newBatteryIconStr = ":/pictures/Battery/battery3.png";
            else if(batteryLvl > 32.5)
                newBatteryIconStr = ":/pictures/Battery/battery2.png";
            else if(batteryLvl > 10)
                newBatteryIconStr = ":/pictures/Battery/battery1.png";
            else
                newBatteryIconStr = ":/pictures/Battery/batteryEmpty.png";

            if (curBatteryIconStr != newBatteryIconStr)
            {
                curBatteryIconStr = newBatteryIconStr;
                actionBatteryIcon->setIcon(QIcon(curBatteryIconStr));
            }
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
    QApplication::quit(); // TODO : called when going to sleep ? Investigate if this is from here (probably not)
}

//===========================================================================
void MainWindow::on_actionHiragana_to_Romanji_MCQ_triggered()
{
    std::cout << "LOG: MainWindow::on_actionHiragana_to_Romanji_QCM_triggered()" << std::endl;
    if (GetMy::Instance().QcmExercicePageInst().InitializeExercice(QcmExerciceType::Hiragana_to_Romanji_MCQ, true))
    {
        ui->ContentStackedWidget->setCurrentIndex(1);
        AggressiveClearScreen();
    }
    else
        GetMy::Instance().ToolsInst()->DisplayPopup(
                "Not enough enabled Hiragana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingsPageInst().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set");
}

void MainWindow::on_actionRomanji_to_Hiragana_MCQ_triggered()
{
    std::cout << "LOG: MainWindow::on_actionRomanji_to_Hiragana_QCM_triggered()" << std::endl;
    if (GetMy::Instance().QcmExercicePageInst().InitializeExercice(QcmExerciceType::Romanji_to_Hiragana_MCQ, true))
    {
        ui->ContentStackedWidget->setCurrentIndex(1);
        AggressiveClearScreen();
    }
    else
        GetMy::Instance().ToolsInst()->DisplayPopup(
                "Not enough enabled Hiragana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingsPageInst().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set");
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
    KanaEditButtonWidget::RefreshJpFixedSizedFnt();
    GetMy::Instance().KanaEditPageInst().InitializeKanaPage(KanaFamilyEnum::hiragana);
    ui->ContentStackedWidget->setCurrentIndex(3);
    DisplayFirstTimeKanasEditPagePopup();
    AggressiveClearScreen();
}



//===========================================================================
void MainWindow::on_actionKatakana_to_Romanji_MCQ_triggered()
{
    std::cout << "LOG: MainWindow::on_actionKatakana_to_Romanji_QCM_triggered()" << std::endl;
    if (GetMy::Instance().QcmExercicePageInst().InitializeExercice(QcmExerciceType::Katakana_to_Romanji_MCQ, true))
    {
        ui->ContentStackedWidget->setCurrentIndex(1);
        AggressiveClearScreen();
    }
    else
        GetMy::Instance().ToolsInst()->DisplayPopup(
                "Not enough enabled Katakana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingsPageInst().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set");
}

void MainWindow::on_actionRomanji_to_Katakana_MCQ_triggered()
{
    std::cout << "LOG: MainWindow::on_actionRomanji_to_Katakana_QCM_triggered()" << std::endl;
    if (GetMy::Instance().QcmExercicePageInst().InitializeExercice(QcmExerciceType::Romanji_to_Katakana_MCQ, true))
    {
        ui->ContentStackedWidget->setCurrentIndex(1);
        AggressiveClearScreen();
    }
    else
        GetMy::Instance().ToolsInst()->DisplayPopup(
                "Not enough enabled Katakana,\nplease enable at least " +
                QString::number(GetMy::Instance().AppSettingsPageInst().GetNumberOfEntry()) +
                " at :\nMain->Hiragana->Edit Hiragana Set");
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
    KanaEditButtonWidget::RefreshJpFixedSizedFnt();
    GetMy::Instance().KanaEditPageInst().InitializeKanaPage(KanaFamilyEnum::katakana);
    ui->ContentStackedWidget->setCurrentIndex(3);
    DisplayFirstTimeKanasEditPagePopup();
    AggressiveClearScreen();
}



//===========================================================================
void MainWindow::on_actionApplication_Setting_triggered()
{
    std::cout << "LOG: MainWindow::on_actionApplication_Setting_triggered()" << std::endl;
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
    if (GetMy::Instance().ScreenSettingsPageInst().AreSettingsAvailablePopup())
    {
        ui->ContentStackedWidget->setCurrentIndex(6);
        AggressiveClearScreen();
    }
}



//===========================================================================
void MainWindow::on_actionVocabulary_to_Romanji_MCQ_triggered()
{
    std::cout << "LOG: MainWindow::on_actionVocabulary_to_Romanji_triggered()" << std::endl;
    if (GetMy::Instance().QcmExercicePageInst().InitializeExercice(QcmExerciceType::Vocabulary_to_Romanji_MCQ, true))
    {
        ui->ContentStackedWidget->setCurrentIndex(1);
        AggressiveClearScreen();
    }
    else
        GetMy::Instance().ToolsInst()->DisplayPopup(
                "Please enable more Vocabulary Sheets at Main->Vocabulary->Learn / Edit words Set\n"
                "At least "+QString::number(GetMy::Instance().AppSettingsPageInst().GetNumberOfEntry())+
                " entries / Vocabulary Lines across all the Sheets/.oben files are required");
}

void MainWindow::on_actionRomanji_to_Vocabulary_MCQ_triggered()
{
    std::cout << "LOG: MainWindow::on_actionVocabulary_to_Romanji_triggered()" << std::endl;
    if (GetMy::Instance().QcmExercicePageInst().InitializeExercice(QcmExerciceType::Romanji_to_Vocabulary_MCQ, true))
    {
        ui->ContentStackedWidget->setCurrentIndex(1);
        AggressiveClearScreen();
    }
    else
        GetMy::Instance().ToolsInst()->DisplayPopup(
                "Please enable more Vocabulary Sheets at Main->Vocabulary->Learn / Edit words Set\n"
                "At least "+QString::number(GetMy::Instance().AppSettingsPageInst().GetNumberOfEntry())+
                " entries / Vocabulary Lines across all the Sheets/.oben files are required");
}

void MainWindow::on_actionLearn_Edit_Set_triggered()
{
    std::cout << "LOG: MainWindow::on_actionLearn_Edit_Set_triggered()" << std::endl;
    GetMy::Instance().VocabExplorerPageInst()->InitializeVocabularyLearnEditSet();
    ui->ContentStackedWidget->setCurrentIndex(7);
    AggressiveClearScreen();

    if ( GetMy::Instance().AppSettingsPageInst().GetSettingsSerializer()->value("AppSettings/firstTimeVocabListPage", true).toBool() )
    {
        GetMy::Instance().ToolsInst()->DisplayPopup("Here are displayed your \"vocab sheets\". Please check Obenkyobo's github page to learn how to create your owns.\n"
                                          "Click on any one to learn/display its content.\n");
        GetMy::Instance().AppSettingsPageInst().GetSettingsSerializer()->setValue("AppSettings/firstTimeVocabListPage", false);
    }
}

//===========================================================================

void MainWindow::on_actionStatistics_triggered()
{
    GetMy::Instance().StatisticsInst()->UpdateScoreCounters();
    ui->ContentStackedWidget->setCurrentIndex(9);
}
