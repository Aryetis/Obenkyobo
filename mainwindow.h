#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
