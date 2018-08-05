#include "joystick.h"
#include <SDL.h>
#include <QDebug>
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;
extern int posq;
extern float UJ,DJ,RJ,LJ,BJ,FJ;

joystick::joystick()
{


    //gGameController = SDL_JoystickOpen( 0 );
    SDL_Joystick *joy;
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    if (SDL_NumJoysticks() > 0)
        {
            // Open joystick
            for(index=0;index<SDL_NumJoysticks();index++)
            {
                if(strcmp(SDL_JoystickNameForIndex(index),"WingMan RumblePad")==0){
                    name="WingMan RumblePad";
                    break;
                }
            }
            joy = SDL_JoystickOpen(index);


            if (joy) {
                cout <<"Opened Joystick 0\n";
                printf("Name: %s\n", SDL_JoystickNameForIndex(index));
                printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
                printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
                printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
            } else {
                printf("Couldn't open Joystick 0\n");
            }

            // Close if opened
            /*if (SDL_JoystickGetAttached(joy)) {
                SDL_JoystickClose(joy);
            }*/
        }

    }

void joystick::run()
{
    cout <<"thread running\n";
    while(1)
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_JOYAXISMOTION )
            {

                if( e.jaxis.axis == 0 )//left side to side
                {
                   if(e.jaxis.value> JOYSTICK_DEAD_ZONE)
                    {
                       front();
                       //qDebug()<< "axis 0, left to the right" << e.jaxis.value/32768<<"\n";
                    }
                   if(e.jaxis.value<-JOYSTICK_DEAD_ZONE)
                   {
                        back();
                        //qDebug() << "axis 0, left to the left" << e.jaxis.value/32768<<"\n";
                   }
                }
                if( e.jaxis.axis == 1 ) //left back and forth
                {
                    if(e.jaxis.value> JOYSTICK_DEAD_ZONE)
                     {
                         //up();
                         //qDebug() << "axis 1,left backwards"<< e.jaxis.value/32768<<"\n";
                         if(e.jaxis.value>30000)
                            BJ=1;
                         else
                            BJ=0;
                     }
                    if(e.jaxis.value<-JOYSTICK_DEAD_ZONE)
                    {
                         //down();
                         //qDebug() << "axis 1, left forward"<< e.jaxis.value/32768<<"\n";
                        if(e.jaxis.value<-30000)
                           FJ=1;
                        else
                           FJ=0;
                    }
                }
                if( e.jaxis.axis == 2 ) //slider rigth and left
                {
                    if(e.jaxis.value> JOYSTICK_DEAD_ZONE)
                     {
                         //right();
                         //qDebug() << "slider  to the left "<<e.jaxis.value/32767<<endl;
                     }
                    if(e.jaxis.value<-JOYSTICK_DEAD_ZONE)
                    {
                         //left();
                         //qDebug() << "slider  to the right "<<e.jaxis.value/32767<<endl;
                    }
                }
                if( e.jaxis.axis == 3 )
                {
                    if(e.jaxis.value> JOYSTICK_DEAD_ZONE)
                     {
                         //up();
                         //qDebug() << "axis 3, right to the right "<<e.jaxis.value/32767;
                        if(e.jaxis.value>30000)
                           RJ=1;
                        else
                           RJ=0;
                     }
                    if(e.jaxis.value<-JOYSTICK_DEAD_ZONE)
                    {
                        //down();
                        //qDebug() << "axis 3, right to the left "<<e.jaxis.value/32767;
                        if(e.jaxis.value<-30000)
                           LJ=1;
                        else
                           LJ=0;
                    }
                }
                if( e.jaxis.axis == 4 )
                {
                    if(e.jaxis.value> JOYSTICK_DEAD_ZONE)
                     {
                         //up();
                         //qDebug() << "right backward axis 4 "<<e.jaxis.value/32767;
                        if(e.jaxis.value>30000)
                           UJ=1;
                        else
                           UJ=0;
                     }
                    if(e.jaxis.value<-JOYSTICK_DEAD_ZONE)
                    {
                        //down();
                        //qDebug() << "right forward axis 4 "<<e.jaxis.value/32767;
                        if(e.jaxis.value<-30000)
                           DJ=1;
                        else
                           DJ=0;
                    }
                }
            }
            if( e.type == SDL_JOYBUTTONDOWN ){
                switch (e.jbutton.button) {
                case 0:
                    up();
                    break;
                case 1:
                    down();
                    break;
                case 2:
                    front();
                    break;
                case 3:
                    back();
                    break;
                case 4:
                    right();
                    break;
                case 5:
                    left();
                    break;
                default:
                    break;
                }

            }

        }
    }
}

 joystick::~joystick(){
    // Close if opened
  //  if (SDL_JoystickGetAttached(joy))
    {
        //SDL_JoystickClose(joy);
    }
}
