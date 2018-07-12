#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_HTP_Button_clicked();

    void on_A_Button_clicked();

    void on_CC_Button_clicked();

    void on_G_Button_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
