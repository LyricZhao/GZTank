#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "htp.h"
#include "about.h"
#include "constatus.h"
#include "logwin.h"

#include <iostream>

extern ConStatus globalConnect;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_HTP_Button_clicked()
{
    HTP *help_win = new HTP;
    help_win -> show();
}

void MainWindow::on_A_Button_clicked()
{
    About *about_win = new About;
    about_win -> show();
}

void MainWindow::on_CC_Button_clicked()
{
    globalConnect.showWindow();
}

void MainWindow::on_G_Button_clicked()
{
    logWin *lW = new logWin;
    lW -> show();
}
