#include "serialcom.h"

SerialCOM::SerialCOM() {
    fd = -1, portName = "Not Connected";
}

int SerialCOM:: connect(std::string serialPath) {
    fd = open(serialPath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1) return -1;

    /* setting speed */
    termios options;
    if(tcgetattr(fd, &options) != 0) {
        mclose();
        return -1;
    }
    tcflush(fd, TCIOFLUSH);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    if(tcsetattr(fd, TCSANOW, &options) != 0) {
        mclose();
        return -1;
    }
    tcflush(fd, TCIOFLUSH);

    /* opened */
    portName = serialPath;
    return 0;
}

bool SerialCOM:: status() {
    return fd != -1;
}

void SerialCOM:: mclose() {
    if(fd != -1) {
        close(fd);
        fd = -1, portName = "Not Connected";
    }
}

int SerialCOM:: senddata(char control) {
    if(fd == -1) return -1;
    int wrn = write(fd, &control, 1);
    return wrn > 0 ? 0 : -1;
}
