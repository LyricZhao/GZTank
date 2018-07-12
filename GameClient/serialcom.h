#ifndef SERIALCOM_H
#define SERIALCOM_H

# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <errno.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <termios.h>
# include <stdlib.h>
# include <iostream>
# include <string>

class SerialCOM {
    int fd;
public:

    std:: string portName;
    SerialCOM();

    bool status();
    int connect(std:: string serialPath);
    void mclose();
    int senddata(char control);
};

#endif // SERIALCOM_H
