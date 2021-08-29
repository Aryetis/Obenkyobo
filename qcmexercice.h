#ifndef QCMEXERCICE_H
#define QCMEXERCICE_H

#include <QWidget>

namespace Ui {
class QcmExercice;
}

class QcmExercice : public QWidget
{
    Q_OBJECT

public:
    explicit QcmExercice(QWidget *parent = nullptr);
    ~QcmExercice();
    void InitializeExercice();
private:
    Ui::QcmExercice *ui;
};

#endif // QCMEXERCICE_H
