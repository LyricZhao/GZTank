#include <QMessageBox>

#include "logwin.h"
#include "keywidget.h"
#include "ui_logwin.h"
#include "gamewindow.h"

logWin::logWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logWin)
{
    ui->setupUi(this);
}

logWin::~logWin()
{
    delete ui;
}

void logWin::on_CloseButton_clicked()
{
    this -> close();
}

void logWin::on_ConnectButton_clicked()
{
    QString serverAddr = ui -> IP_INPUT -> text();
    QString userName = ui -> USR_INPUT -> text();

    if(serverAddr.size() == 0 || userName.size() == 0) {
        QMessageBox:: information(this, tr("QMessageBox::information("), "The server address or your username can not be empty.");
        return;
    }

    GameWindow *gW = new GameWindow(serverAddr, userName);
    gW -> show();
    this -> close();
}
