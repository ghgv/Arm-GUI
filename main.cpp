#include "mainwindow.h"
#include "loadfile.h"
#include "bad.h"
#include <QApplication>
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


int posq;
int pos2;

float degreeZ;


int main(int argc, char *argv[])
{



    QApplication a(argc, argv);
    MainWindow w;
    pathStr= a.applicationDirPath();


    QSurfaceFormat format;
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        format.setVersion(3, 2);
        format.setProfile(QSurfaceFormat::CoreProfile);
        QSurfaceFormat::setDefaultFormat(format);

    QGLFormat glFormat;
     glFormat.setSampleBuffers(true);
     glFormat.setSamples(4);






    w.show();



    return a.exec();
}
