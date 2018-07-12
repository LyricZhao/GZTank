#include <cassert>
#include <iostream>

#include "keywidget.h"
#include "constatus.h"

extern ConStatus globalConnect;

char indexToKey[4] = {'W', 'S', 'A', 'D'}; //0, 1, 2, 3

int commandMap[16] = {
    's', // 0000
    'f', // 0001 w
    'b', // 0010 s
    's', // 0011 ws
    'l', // 0100 a
    'l', // 0101 wa
    'r', // 0110 sa
    'l', // 0111 wsa
    'r', // 1000 d
    'r', // 1001 wd
    'l', // 1010 sd
    'r', // 1011 wsd
    's', // 1100 ad
    'f', // 1101 wad
    'b', // 1110 sad
    's'  // 1111 wasd
};

inline int dirConvert(char key) {
    for(int i = 0; i < 4; i ++) {
        if(indexToKey[i] == key) return i;
    }
    return -1;
}

inline bool isDir(char key) {
    return key == 'W' || key == 'A' || key == 'S' || key == 'D';
}

inline bool isShoot(char key) {
    return key == ' ' || key == 'X' || key == 'Z';
}

void KeyWidget:: keyPressEvent(QKeyEvent *ev) {
    char key = ev -> key();
    if('a' <= key && key <= 'z') key ^= 32;
    std:: cout << "pressing key: " << key << (char)key << std:: endl;
    if(isDir(key)) {
        int index = dirConvert(key);
        assert(index != -1);

        keystatus |= 1 << index;
        char newState = commandMap[keystatus];
        if(newState != lastKey) {
            globalConnect.send(newState);
            lastKey = newState;
        }
    } else if(isShoot(key)) {
        switch(key) {
            case ' ':
                emit shootSignal();
                break;
            case 'X':
                emit FlashSignal();
                break;
            case 'Z':
                emit EMPSignal();
                break;
        }
    }
}

void KeyWidget:: keyReleaseEvent(QKeyEvent *ev) {
    char key = ev -> key();
    if('a' <= key && key <= 'z') key ^= 32;
    // std:: cout << "releasing key: " << key << (char)key << std:: endl;
    if(isDir(key)) {
        int index = dirConvert(key);
        assert(index != -1);

        keystatus &= ((-1) ^ (1 << index));
        char newState = commandMap[keystatus];
        if(newState != lastKey) {
            globalConnect.send(newState);
            lastKey = newState;
        }
    }
}

KeyWidget::KeyWidget(QWidget *parent): QWidget(parent) {
    keystatus = 0;
    lastKey = 's';
}
