#ifndef LOGWIN_H
#define LOGWIN_H

#include <QDialog>

namespace Ui {
class logWin;
}

class logWin : public QDialog
{
    Q_OBJECT

public:
    explicit logWin(QWidget *parent = 0);
    ~logWin();

private slots:
    void on_CloseButton_clicked();

    void on_ConnectButton_clicked();

private:
    Ui::logWin *ui;
};

#endif // LOGWIN_H
