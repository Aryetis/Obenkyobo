#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include "koboplatformfunctions.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void UpdateStatusBarGeometry() const;
    void SwitchStackedWidgetIndex(int i);
    void AggressiveClearScreen() const;
    void OnSleep() const;
    void OnWakeUp();

private slots:
    //===========================================================================
    void on_actionAbout_triggered();
    void on_actionExit_triggered();

    //===========================================================================
    void on_actionHiragana_to_Romanji_QCM_triggered();
    void on_actionRomanji_to_Hiragana_QCM_triggered();
    void on_actionHiragana_to_Romanji_Kbd_triggered();
    void on_actionEdit_Hiragana_Set_triggered();

    //===========================================================================
    void on_actionKatakana_to_Romanji_QCM_triggered();
    void on_actionRomanji_to_Katakana_QCM_triggered();
    void on_actionKatakana_to_Romanji_Kbd_triggered();
    void on_actionEdit_Katakana_Set_triggered();

    //===========================================================================
    void on_actionApplication_Setting_triggered();
    void on_actionFonts_Setting_triggered();
    void on_actionScreen_Setting_triggered();

    //===========================================================================
    void on_actionKana_To_Romanji_MCQ_triggered();
    void on_actionRomanji_to_Kanas_MCQ_triggered();
    void on_actionLearn_Edit_Set_triggered();

    //===========================================================================
    void refreshTimeAndBattery();

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void DisplayFirstTimeMainWindowPagePopup();
    void DisplayFirstTimeKanasEditPagePopup();

    Ui::MainWindow *ui;

    QMenuBar* statusBar;
    QAction* timeDisplay;
    QAction* actionBatteryIcon;
    QAction* actionBatteryTxt;

    QTimer timer;
    int wasBatteryLvl;
    int wasBatteryDisplayFormat;
    int wasBatteryCharging;

    bool firstTimeMainWindow;
};

#endif // MAINWINDOW_H
