#ifndef TOOLS_H
#define TOOLS_H
#include <random>
#include <iterator>
#include <QString>
#include <QMessageBox>

#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QLayout>

#include <QPushButton>
#include <QPainter>

#include "lockscreen.h"

class Tools
{
public :
    static Tools & GetInstance()
    {
        static Tools instance;
        return instance;
    }

    //======================================================================
    template<typename Iter, typename RandomGenerator>
    Iter GetRandom(Iter start, Iter end, RandomGenerator& gen)
    {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(gen));
        return start;
    }

    template<typename Iter>
    Iter GetRandom(Iter start, Iter end)
    {
        return GetRandom(start, end, rng_engine);
    }

    int GetRandomInt(int rangeStart = 0, int rangeEnd = 100)
    {
        std::uniform_int_distribution<> distr(rangeStart, rangeEnd);
        return distr(rng_engine);
    }

    //======================================================================
    void DisplayPopup(QString message)
    {
        QMessageBox popup;
        popup.setText(message);
        popup.setStyleSheet("QMessageBox { border: 3px solid black }");
        popup.exec();
    }

    std::mt19937& MT() { return mt; }
    std::default_random_engine& Rng_Engine() { return rng_engine; }
    void Sleeping(bool _b) { sleeping = _b; }
    bool Sleeping() const { return sleeping; }



//    class LockscreenDialog : public QDialog {
//    public:
//      LockscreenDialog(QWidget *parent=0):QDialog(parent) {
//        QGridLayout *grid = new QGridLayout();
//        setLayout(grid);
//        QPushButton *startB = new QPushButton("Start Me", this);
//        grid->addWidget(startB,0,0,Qt::AlignCenter);
//        pixmap = QPixmap("mybackground.png");
//      }
//    protected:
//      void resizeEvent(QResizeEvent *re) {
//        pixmap  = pixmap.scaled(re->size(),Qt::KeepAspectRatioByExpanding);
//      }
//      void paintEvent(QPaintEvent *re) {
//        QPainter painter(this);
//        painter.drawPixmap(0,0,pixmap);
//      }
//    private:
//      QPixmap pixmap;
//    };


    void Sleep()
    {
        if (sleeping)
            return;

        Tools::GetInstance().DisplayPopup("Let's go to sleep");

//        QDialog lockscreen;
//        lockscreen.setWindowState(Qt::WindowFullScreen);

//        lockscreen.setStyleSheet("background: black");
//        lockscreen.exec();

////        QLabel *lockImage = new QLabel();
////        lockImage->setPixmap(QPixmap (":/pictures/pictures/lockscreen.jpg"));
////        lockImage->setScaledContents(true);
////        lockImage->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

////        lockscreen.layout()->addWidget(lockImage);
//        lockscreen.exec();

        sleeping = true;
    }

    void WakeUp()
    {
        if (!sleeping)
            return;

        sleeping = false;
    }

private :
    //======================================================================
    Tools()
    {
        mt = std::mt19937(rd_device());
        rng_engine = std::default_random_engine{};
        sleeping = false;
    }

    std::random_device rd_device;
    std::mt19937 mt;
    std::default_random_engine rng_engine;

    bool sleeping;
};

#endif // TOOLS_H
