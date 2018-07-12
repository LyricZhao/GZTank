#ifndef KEYWIDGET_H
#define KEYWIDGET_H

#include <QWidget>
#include <QObject>
#include <QKeyEvent>

class KeyWidget: public QWidget {
    Q_OBJECT

private:
    int keystatus; // udlr
    char lastKey;

public:
    explicit KeyWidget(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

signals:
    void shootSignal();
    void EMPSignal();
    void FlashSignal();

public slots:

};

#endif // KEYWIDGET_H
