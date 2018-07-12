#ifndef CONSTATUS_H
#define CONSTATUS_H

#include <string>
#include <vector>

#include "serialcom.h"

class ConStatus {
private:
    SerialCOM bluebooth;
    std:: string wifi_addr;
    std:: vector<std:: string> getSerialList();

public:
    ConStatus();
    void showWindow();
    std:: string getCurrentPort();
    std:: string getWiFiAddress();
    void connectToPort(std:: string portName);
    void closePort();
    void setWiFiAddress(std:: string wifi_url);
    void send(char control);
};

#endif // CONSTATUS_H
