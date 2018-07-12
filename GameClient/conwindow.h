#ifndef CONWINDOW_H
#define CONWINDOW_H

#include <QDialog>
#include <vector>
#include <string>

namespace Ui {
class ConWindow;
}

class ConWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConWindow(QWidget *parent = 0);
    ~ConWindow();

    void setSerialList(const std:: vector<std:: string> &serialList);
    void setCurrentSerialPort(std:: string serialName);

private slots:
    void on_ConnectButton_clicked();

    void on_SaveButton_clicked();

private:
    Ui::ConWindow *ui;
};

#endif // CONWINDOW_H
