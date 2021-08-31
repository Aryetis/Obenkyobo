#ifndef QCMENTRYGUESS_H
#define QCMENTRYGUESS_H

#include <QWidget>
#include <QString>

namespace Ui {
class QcmEntryGuess;
}

class QcmEntryGuess : public QWidget
{
    Q_OBJECT

public:
    explicit QcmEntryGuess(QWidget *parent = nullptr);
    ~QcmEntryGuess();

    void SetGuess(QString s, bool b);

private slots:
    void on_EntryGuess_clicked();

private:
    Ui::QcmEntryGuess *ui;
    bool correctGuess;
};

#endif // QCMENTRYGUESS_H
