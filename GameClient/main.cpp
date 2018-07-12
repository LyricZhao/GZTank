#include <QApplication>

#include "mainwindow.h"
#include "constatus.h"
#include "keywidget.h"

ConStatus globalConnect;

// # define MW_KEY_CONNECT_TEST

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
#ifdef MW_KEY_CONNECT_TEST
    KeyWidget keycontroller(&w);

    keycontroller.setVisible(false);
    w.setMenuWidget(&keycontroller);

    keycontroller.setFocusPolicy(Qt::StrongFocus);
    keycontroller.show();
#endif
    w.show();

    return a.exec();
}
