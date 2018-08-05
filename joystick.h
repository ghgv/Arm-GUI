#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QObject>
#include <QThread>
#include "SDL.h"

class joystick : public QThread
{
     Q_OBJECT
    SDL_Event e;
    const int JOYSTICK_DEAD_ZONE = 3000;
    int index=0;

public:
    QString name="";
    joystick();
    //~joystick();
    ~joystick();
protected:
    void run();
Q_SIGNALS:
    void up();
    void down();
    void front();
    void back();
    void right();
    void left();


};

#endif // JOYSTICK_H
