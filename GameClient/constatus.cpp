#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <QDialog>
#include <QObject>
#include <QErrorMessage>

#include "utility.h"
#include "constatus.h"
#include "conwindow.h"

ConStatus::ConStatus() {
    wifi_addr = "192.168.4.1";
}

std:: string ConStatus:: getCurrentPort() {
    return bluebooth.portName;
}

std:: vector<std:: string> ConStatus:: getSerialList() {
    std:: vector<std:: string> ports = simLs("/dev/");
    // std:: cout << "ports:" << std:: endl;
    // for(auto it: ports) std:: cout << it << std:: endl;
    std:: vector<std:: string> bfilter = filter(ports, std:: string("tty."));
    return bfilter;
}

void ConStatus:: showWindow() {
    ConWindow *cw = new ConWindow;
    cw -> setSerialList(getSerialList());
    cw -> setCurrentSerialPort(bluebooth.portName);
    cw -> show();
}

void ConStatus:: connectToPort(std::string portName) {
    portName = "/dev/" + portName;
    int ret = bluebooth.connect(portName);
    if(ret == -1) {
        // failed
        QErrorMessage *error_print = new QErrorMessage;
        error_print -> setWindowTitle("Error");
        error_print -> showMessage("Unable to connect.");
    }
}

void ConStatus:: setWiFiAddress(std::string wifi_url) {
    wifi_addr = wifi_url;
}

std:: string ConStatus:: getWiFiAddress() {
    return wifi_addr;
}

void ConStatus:: closePort() {
    bluebooth.mclose();
}

void ConStatus:: send(char control) {
    // std:: cout << "trying to send " << control << std:: endl;
    bluebooth.senddata(control);
}
