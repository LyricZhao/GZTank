#include "conwindow.h"
#include "ui_conwindow.h"

#include "constatus.h"

extern ConStatus globalConnect;

ConWindow::ConWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConWindow)
{
    ui->setupUi(this);
}

ConWindow::~ConWindow()
{
    delete ui;
}

void ConWindow:: setSerialList(const std::vector<std::string> &serialList){
    for(auto item: serialList) {
        this -> ui -> comboBox -> addItem(QString:: fromStdString(item));
    }
}

void ConWindow:: setCurrentSerialPort(std::string serialName) {
    this -> ui -> CUR -> setText(QString:: fromStdString(serialName));
}

void ConWindow::on_ConnectButton_clicked() {
    QString port = this -> ui -> comboBox -> currentText();
    std:: string portStd = port.toStdString();
    globalConnect.connectToPort(portStd);
    setCurrentSerialPort(globalConnect.getCurrentPort());
}

void ConWindow::on_SaveButton_clicked() {
    QString url = this -> ui -> WiFiAddress -> text();
    globalConnect.setWiFiAddress(url.toStdString());
}
