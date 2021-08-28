#ifndef QCMENTRYGUESS_H
#define QCMENTRYGUESS_H

#include <QWidget>

namespace Ui {
class QcmEntryGuess;
}

class QcmEntryGuess : public QWidget
{
    Q_OBJECT

public:
    explicit QcmEntryGuess(QWidget *parent = nullptr);
    ~QcmEntryGuess();

    void SetGuessText(std::string s);

private:
    Ui::QcmEntryGuess *ui;
};

#endif // QCMENTRYGUESS_H
