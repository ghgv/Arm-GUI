#include "mainwindow.h"
#include "loadfile.h"
#include "bad.h"
#include "camino.h"
#include "uarm.h"

#include <QApplication>
#include <stdio.h>
#include <QOpenGLWidget>
#include <QGLFormat>
//#include <QtDeclarative/qdeclarativeview.h>
#include <QDebug>


#include <QOpenGLWindow>
#include <QScreen>
#include <QPainter>
#include <QGuiApplication>
#include <QMatrix4x4>
#include <QStaticText>
#include <QKeyEvent>

QString pathStr;





float angle = 0.0f;
Bad *Gear2 =new Bad();
Bad *main_gear =new Bad();
Bad *main_gear3 =new Bad();
Bad *main_bar= new Bad();
Bad *arm1= new Bad();
Bad *arm2= new Bad();
Bad *arm3= new Bad();
Bad *small_gear1 = new Bad();
Bad *small_gear2 = new Bad();
Bad *small_gear3 = new Bad();
Bad *upper_triangle =new Bad();
Bad *motor1 =new Bad();
Bad *motor2 =new Bad();
Bad *motor3 =new Bad();
Bad *horizontal = new Bad();
Bad *base = new Bad();
Bad *bottom_plate = new Bad();
Bad *left1 = new Bad();
Bad *rigth1 = new Bad();
Bad *palanca2 = new Bad();
Bad *sphere = new Bad();
Bad *secondtriangle = new Bad();
Bad *SG90 = new Bad();
Bad *SG90holder = new Bad();
Bad *plane = new Bad();
Bad *esfera;


int posx;
int pos2;
int cameraposZ;
int solidos=0;

bool rtopview    =   true;
bool rfrontview  =   false;

bool UARM=true, DARM=false;
bool joystick_found=true;

float degreeZ;
camino *kamino;
float UJ=0,DJ=0,RJ=0,LJ=0,FJ=0,BJ=0;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseDesktopOpenGL);
    MainWindow w;
    pathStr= a.applicationDirPath();
    kamino= new camino(pathStr);
    w.show();
    return a.exec();
}
