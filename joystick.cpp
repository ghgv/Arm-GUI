#include "joystick.h"
#include <SDL.h>
#include <stdio.h>
#include <iostream>

using namespace std;
extern int posq;

joystick::joystick()
{


    //gGameController = SDL_JoystickOpen( 0 );
    SDL_Joystick *joy;
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    if (SDL_NumJoysticks() > 0)
        {
                    // Open joystick
            joy = SDL_JoystickOpen(0);

            if (joy) {
                cout <<"Opened Joystick 0\n";
                printf("Name: %s\n", SDL_JoystickNameForIndex(0));
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
                if( e.jaxis.axis == 0 )//y AXIS
                {
                   if(e.jaxis.value> JOYSTICK_DEAD_ZONE)
                    {
                       front();cout << "axis 0" << e.jaxis.value<<"\n";
                    }
                   if(e.jaxis.value<-JOYSTICK_DEAD_ZONE)
                   {
                        back();cout << "axis -0" << e.jaxis.value<<"\n";
                   }
                }
                if( e.jaxis.axis == 1 )
                {
                    if(e.jaxis.value> JOYSTICK_DEAD_ZONE)
                     {
                         up();cout << "axis 1"<< e.jaxis.value<<"\n";
                     }
                    if(e.jaxis.value<-JOYSTICK_DEAD_ZONE)
                    {
                         down();cout << "axis -1"<< e.jaxis.value<<"\n";
                    }
                }
                if( e.jaxis.axis == 2 )
                {
                    if(e.jaxis.value> JOYSTICK_DEAD_ZONE)
                     {
                         right();cout << "axis 2\n";
                     }
                    if(e.jaxis.value<-JOYSTICK_DEAD_ZONE)
                    {
                         left();cout << "axis -2\n";
                    }
                }
                if( e.jaxis.axis == 3 )
                {
                    if(e.jaxis.value> JOYSTICK_DEAD_ZONE)
                     {
                         cout << "axis 3\n";
                     }
                    if(e.jaxis.value<-JOYSTICK_DEAD_ZONE)
                    {
                        cout << "axis -3\n";
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


