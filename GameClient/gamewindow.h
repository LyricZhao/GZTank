#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QImage>
#include <QDialog>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>

#include <vector>
#include <string>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "keywidget.h"
#include "facetracker.h"

#define D_PORT 8000
#define G_PORT 8001

#define ON_SERVER_PRINT 0

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameWindow(QString gameServerURL, QString iUserName, QWidget *parent = 0);
    ~GameWindow();
    void socketSender(char command);
    void consolePrint(std:: string info, bool islocal = true);
    void consolePrint(QString info, bool islocal = true);

private:
    bool serverOnline;
    bool inCenter, useOpenCV;
    int blood, shootTime, empTime, flashTime, cameraTime;

    QTcpSocket *JPG_Stream;
    QTcpSocket *server_Stream;
    Ui::GameWindow *ui;
    KeyWidget *keyw;
    QByteArray dataBuffer, gDataBuffer;
    faceTracker *faceDetect;
    int toRead, gToRead;
    QString userName;

    QPixmap OpenCV_process(const char *data, int size);
    void gServerInfoProcess(char *data);

private slots:
    void JPG_connected();
    void JPG_readyRead();

    void server_connected();
    void server_readyRead();
    void server_disconnected();

    // actions
    void Keyboard_shootSignal();
    void Keyboard_EMPSignal();
    void Keyboard_FlashSignal();
};

#endif // GAMEWINDOW_H
