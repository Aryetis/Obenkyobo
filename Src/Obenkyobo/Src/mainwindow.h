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
    void AggressiveClearScreen(bool force = false) const;
    void OnSleep() const;
    void OnWakeUp();
    int GetStackedWidgetIdx() const;

private slots:
    //===========================================================================
    void on_actionAbout_triggered();
    void on_actionExit_triggered();

    //===========================================================================
    void on_actionHiragana_to_Romanji_MCQ_triggered();
    void on_actionRomanji_to_Hiragana_MCQ_triggered();
    void on_actionHiragana_to_Romanji_Kbd_triggered();
    void on_actionEdit_Hiragana_Set_triggered();

    //===========================================================================
    void on_actionKatakana_to_Romanji_MCQ_triggered();
    void on_actionRomanji_to_Katakana_MCQ_triggered();
    void on_actionKatakana_to_Romanji_Kbd_triggered();
    void on_actionEdit_Katakana_Set_triggered();

    //===========================================================================
    void on_actionApplication_Setting_triggered();
    void on_actionFonts_Setting_triggered();
    void on_actionScreen_Setting_triggered();

    //===========================================================================
    void on_actionVocabulary_to_Romanji_MCQ_triggered();
    void on_actionRomanji_to_Vocabulary_MCQ_triggered();
    void on_actionLearn_Edit_Set_triggered();

    //===========================================================================
    void on_actionStatistics_triggered();

    //===========================================================================
    void refreshTimeAndBattery();

    void on_actionNotes_triggered();

private:
    void DisplayFirstTimeMainWindowPagePopup();
    void DisplayFirstTimeKanasEditPagePopup();

    void on_refresh_triggered();
    void on_refresh_hovered();

    Ui::MainWindow *ui;

    QMenuBar* statusBar;
    QAction* timeDisplay;
    QAction* actionBatteryIcon;
    QAction* actionBatteryTxt;
    QAction* refresh;

    QTimer timer;
    int wasBatteryLvl;
    int wasBatteryDisplayFormat;
    int wasBatteryCharging;
    QString curBatteryIconStr;

    bool firstTimeMainWindow;
};

#endif // MAINWINDOW_H
