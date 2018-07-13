#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T23:31:32
#
#-------------------------------------------------

QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GZTank
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# The following lines tells Qmake to use pkg-config for opencv
QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig
PKGCONFIG += opencv

LIBS += -L/usr/local/lib/ -lopencv_core -lopencv_imgproc -lopencv_objdetect

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    htp.cpp \
    about.cpp \
    constatus.cpp \
    conwindow.cpp \
    utility.cpp \
    serialcom.cpp \
    keywidget.cpp \
    logwin.cpp \
    gamewindow.cpp \
    facetracker.cpp

HEADERS += \
        mainwindow.h \
    htp.h \
    about.h \
    constatus.h \
    conwindow.h \
    utility.h \
    serialcom.h \
    keywidget.h \
    logwin.h \
    gamewindow.h \
    facetracker.h

FORMS += \
        mainwindow.ui \
    htp.ui \
    about.ui \
    conwindow.ui \
    logwin.ui \
    gamewindow.ui

RESOURCES += \
    resource.qrc
