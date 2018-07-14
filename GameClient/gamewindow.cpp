#include <iostream>
#include <cstring>
#include <cstdlib>

#include <QFileInfo>
#include <QPainter>
#include <QBuffer>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "constatus.h"

extern ConStatus globalConnect;

// # define OPENCV_TEST

GameWindow::GameWindow(QString gameServerURL, QString iUserName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    // for safety
    serverOnline = false;
    server_Stream = NULL;
    JPG_Stream = NULL;
    keyw = NULL;

    // Bluebooth controller connect
    keyw = new KeyWidget;
    keyw -> setVisible(false);
    keyw -> setFocusPolicy(Qt:: StrongFocus);
    connect(this -> keyw, SIGNAL(shootSignal()), this, SLOT(Keyboard_shootSignal()));
    connect(this -> keyw, SIGNAL(EMPSignal()), this, SLOT(Keyboard_EMPSignal()));
    connect(this -> keyw, SIGNAL(FlashSignal()), this, SLOT(Keyboard_FlashSignal()));
    this -> setExtension(keyw);
    this -> showExtension(true);

    ui -> setupUi(this);
    consolePrint(QString("Bluebooth controller load successfully."));

    // OpenCV init
    inCenter = false, useOpenCV = false;
    QMessageBox:: information(NULL, tr("INFO"), "Please choose the OpenCV detection file first.");
    faceDetect = new faceTracker;
    QString cvFilePath = QFileDialog:: getOpenFileName(this, tr("Open CV File"), " ",  tr("XML File(*.xml)"));
    if(cvFilePath.isEmpty() || cvFilePath.isNull()) {
        consolePrint(QString("You are not using OpenCV."));
        goto PASS_OPENCV;
    }
    useOpenCV = true;
    faceDetect -> setCascadeFile(cvFilePath);
    consolePrint(QString("OpenCV starts successfully."));
    PASS_OPENCV:

    // connect to wifi camera server
    JPG_Stream = new QTcpSocket(this);
    std:: string url = globalConnect.getWiFiAddress();
    QString Q_addr = QString:: fromStdString(url);
    JPG_Stream -> connectToHost(Q_addr, D_PORT);
    // std:: cout << url << std:: endl;
    connect(this -> JPG_Stream, SIGNAL(connected()), this, SLOT(JPG_connected()));
    connect(this -> JPG_Stream, SIGNAL(readyRead()), this, SLOT(JPG_readyRead()));
    consolePrint(QString("WiFi connection part starts successfully."));
    consolePrint(QString("Your camera address is ") + Q_addr + ".");

    // connect to game server
    shootTime = empTime = flashTime = 1000;
    userName = iUserName;
    serverOnline = false;
    server_Stream = new QTcpSocket(this);
    server_Stream -> connectToHost(gameServerURL, G_PORT);
    connect(this -> server_Stream, SIGNAL(connected()), this, SLOT(server_connected()));
    connect(this -> server_Stream, SIGNAL(disconnected()), this, SLOT(server_disconnected()));
    connect(this -> server_Stream, SIGNAL(readyRead()), this, SLOT(server_readyRead()));
    consolePrint(QString("Game server connection part starts successfully."));
    consolePrint(QString("Your server address is ") + gameServerURL + ".");
    consolePrint(QString("Your username is ") + userName + ".");

#ifdef OPENCV_TEST
    QImage item("/Users/stdafx/Desktop/capc.jpg");
    QByteArray testArr;
    QBuffer buffer(&testArr);
    buffer.open(QIODevice:: WriteOnly);
    item.save(&buffer, "jpg");
    ui -> Camera -> setPixmap(OpenCV_process(testArr.data(), testArr.size()));
#endif
}

GameWindow::~GameWindow() {
    if(serverOnline)
        socketSender('o');

    if(server_Stream != NULL)
        delete server_Stream;

    if(JPG_Stream != NULL)
        delete JPG_Stream;

    if(keyw != NULL)
        delete keyw;

    delete ui;
}

void GameWindow:: JPG_connected() {
    // std:: cout << "successfully connected" << std:: endl;
    consolePrint(QString("WiFi Camera connects successfully."));
    toRead = 0;
}

QImage IplImageToQImage(const IplImage *pIplImage) {
    int w = pIplImage -> width;
    int h = pIplImage -> height;

    QImage qImage(w, h, QImage::Format_RGB32);

    for(int x = 0; x < pIplImage -> width; ++ x) {
        for(int y = 0; y < pIplImage -> height; ++ y) {
            CvScalar color = cvGet2D(pIplImage, y, x);

            int r = color.val[2];
            int g = color.val[1];
            int b = color.val[0];

            qImage.setPixel(x, y, qRgb(r,g,b));
        }
    }

    return qImage;
}

# define ENABLE_FACE_DETECTION

QPixmap GameWindow:: OpenCV_process(const char *data, int size) {
    // opencv
    std:: vector<unsigned char> ImVec(data, data + size);
    cv:: Mat ImMat = cv:: imdecode(ImVec, 1);
    IplImage proImage(ImMat);


#ifdef ENABLE_FACE_DETECTION
    QVector<QRect> faces;
    if(useOpenCV)
        faces = faceDetect -> detect(&proImage);
#endif

    // painter
    QPixmap pMap = QPixmap:: fromImage(IplImageToQImage(&proImage));

#ifdef ENABLE_FACE_DETECTION
    QPainter painter(&pMap);
    QPen pen(palette().light().color(), 2, Qt:: SolidLine, Qt:: FlatCap, Qt:: BevelJoin);
    painter.setPen(pen);
    painter.drawLine(0, pMap.height() / 2, pMap.width(), pMap.height() / 2);
    painter.drawLine(pMap.width() / 2, 0, pMap.width() / 2, pMap.height());
    int inCounter = 0;
    for(QRect rect: faces) {
        if(rect.contains(pMap.width() / 2, pMap.height() / 2))
            ++ inCounter;
        painter.drawEllipse(rect);
    }
    inCenter = (inCounter > 0);
#endif

    return pMap;
}

void GameWindow:: JPG_readyRead() {
    // std:: cout << "received data" << std:: endl;
    QByteArray picData = JPG_Stream -> readAll();
    dataBuffer.append(picData);

    if(toRead == 0) {
        int *trSize = (int *) dataBuffer.data();
        toRead = *trSize;
        dataBuffer.remove(0, 4);
        // std:: cout << toRead << std:: endl;
    }

    assert(toRead > 0);

    if(dataBuffer.size() > toRead) {
        QPixmap image = OpenCV_process(dataBuffer.data(), toRead);
        if(cameraTime == 0)
            ui -> Camera -> setPixmap(image);
        dataBuffer.remove(0, toRead);
        toRead = 0;
    }
}

void GameWindow:: consolePrint(std:: string info, bool islocal) {
    if(islocal) {
        this -> ui -> console -> append("[LOCAL] " + QString:: fromStdString(info));
    } else {
        this -> ui -> console -> append("[SERVER] " + QString:: fromStdString(info));
    }

}

void GameWindow:: consolePrint(QString info, bool islocal) {
    if(islocal) {
        this -> ui -> console -> append("[LOCAL] " + info);
    } else {
        this -> ui -> console -> append("[SERVER] " + info);
    }
}

void GameWindow:: server_connected() {
    serverOnline = true;
    consolePrint(QString("Game server is connected successfully."), ON_SERVER_PRINT);

    // then ready to send username
    int name_size = userName.size() + 1;
    int buffer_size = 1 + 4 + name_size; // u + int(size) + name + '\0'
    char *buffer = (char *) malloc(buffer_size);
    buffer[0] = 'u';
    *((int *)(buffer + 1)) = name_size;
    std:: string std_name = userName.toStdString();
    memcpy(buffer + 5, std_name.c_str(), name_size);
    server_Stream -> write(buffer, buffer_size);
    free(buffer);
}

void GameWindow:: server_disconnected() {
    serverOnline = false;
}

void GameWindow:: socketSender(char command) {
    std:: cout << "sending " << command << std:: endl;
    if(serverOnline)
        server_Stream -> write(&command, 1);
    std:: cout << "send complete" << std:: endl;
}

/*
 * Code:
    Shoot: s
    EMP: e 10
    Flash: f 10
    size+(0+blood+shoot+emp+flash+camera) (type=0) 1s
    size+(1+string) (type=1)
*/

void GameWindow:: gServerInfoProcess(char *data) {
    int *int_buffer = (int *)data;
    int msgType = int_buffer[0];
    if(msgType == 0) { // status

        // collect info
        QString statusBarUpdate = "";
        statusBarUpdate += "Blood: " + QString:: number(int_buffer[1]) + "  ";
        statusBarUpdate += "Shoot Countdown: " + QString:: number(int_buffer[2]) + "  ";
        statusBarUpdate += "EMP Countdown: " + QString:: number(int_buffer[3]) + "  ";
        statusBarUpdate += "Flash Countdown: " + QString:: number(int_buffer[4]) + "  ";
        statusBarUpdate += "Camera: " + (int_buffer[5] == 0 ? QString("Online") : QString("Offline"));

        blood = int_buffer[1];
        shootTime = int_buffer[2];
        empTime = int_buffer[3];
        flashTime = int_buffer[4];
        cameraTime = int_buffer[5];

        if(int_buffer[1] <= 0) {
            statusBarUpdate = "DEAD";
        }

        // update info
        ui -> StautsBar -> setText(statusBarUpdate);

    } else { // system message
        data += 4;
        QString serverMsg(data);
        consolePrint(serverMsg, ON_SERVER_PRINT);
    }
}

void GameWindow:: server_readyRead() {
    QByteArray gameData = server_Stream -> readAll();
    gDataBuffer.append(gameData);

    if(gToRead == 0) {
        int *trSize = (int *) gDataBuffer.data();
        gToRead = *trSize;
        gDataBuffer.remove(0, 4);
    }

    assert(gToRead > 0);
    if(gDataBuffer.size() > gToRead) {
        gServerInfoProcess(gDataBuffer.data());
        gDataBuffer.remove(0, gToRead);
        gToRead = 0;
    }
}

# define NO_LIMT_SHOOT

// actions below
void GameWindow:: Keyboard_shootSignal() {
    if(serverOnline == false) {
        consolePrint(QString("You are not online."));
        return;
    }

#ifndef NO_LIMT_SHOOT
    if(blood <= 0) {
        consolePrint(QString("You have already died."));
        return;
    }

    if(shootTime > 0) {
        consolePrint(QString("Cannon is freezed now."));
        return;
    }

    if(!inCenter) {
        consolePrint(QString("Not aiming."));
        return;
    }
#endif

    consolePrint(QString("Fire!!!"));
    socketSender('s');
}

void GameWindow:: Keyboard_EMPSignal() {
    if(serverOnline == false) {
        consolePrint(QString("You are not online."));
        return;
    }

#ifndef NO_LIMT_SHOOT
    if(blood <= 0) {
        consolePrint(QString("You have already died."));
        return;
    }

    if(empTime > 0) {
        consolePrint(QString("EMP is freezed now."));
        return;
    }

    if(!inCenter) {
        consolePrint(QString("Not aiming"));
        return;
    }
#endif

    empTime = 1000;
    consolePrint(QString("EMP shock fired!!!"));
    socketSender('e');
}

void GameWindow:: Keyboard_FlashSignal() {
    if(serverOnline == false) {
        consolePrint(QString("You are not online."));
        return;
    }

#ifndef NO_LIMT_SHOOT
    if(blood <= 0) {
        consolePrint(QString("You have already died."));
        return;
    }

    if(flashTime > 0) {
        consolePrint(QString("Flash is freezed now."));
        return;
    }

    if(!inCenter) {
        consolePrint(QString("Not aiming"));
        return;
    }
#endif

    flashTime = 1000;
    consolePrint(QString("Flash fired!!!"));
    socketSender('f');
}
