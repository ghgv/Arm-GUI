#include <QApplication>
#include <QMessageBox>
#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include <QThread>
//include <pybind11/pybind11.h>
#include <pybind11/embed.h> // everything needed for embedding


#include "mainwindow.h"
#include "joystick.h"
#include "camino.h"
#include "ui_mainwindow.h"
#include "uarmext.h"

#include <QSerialPort>
#include <QTimer>
#include <string.h>
#include <Python.h>
#include "helper.h"
#include "bad.h"
#include "loadfile.h"


QSerialPort *serial;
QTimer *timer;
namespace py = pybind11;
joystick *joy;
extern camino *kamino;
extern float UJ,DJ,RJ,LJ,BJ,FJ;

QString str;
float yPOS=0;
float xPOS=0;
float zPOS=0;

float X1=2,Y1=-1,Z1=3;
float gamma=0;
bool connected=0;
float a1=0,b1=12,c1=12;

bool feedbackb=false;
bool ORTHO=true;
bool PROJECTED;

#define ratio 4.0909
#define RED 0xFF3333
#define GREEN 0x99FF33
#define BLACK 0x0
#define YELLOW 0xFFFF00
#define WHITE 0xFFFFFF
#define GRAY 0xC0C0C0
#define BLUE 0x0000FF

extern Bad *Gear2;
extern Bad *main_gear;
extern Bad *main_gear3;
extern Bad *main_bar;
extern Bad *arm1;
extern Bad *arm2;
extern Bad *arm3;
extern Bad *small_gear1;
extern Bad *small_gear2;
extern Bad *small_gear3;
extern Bad *upper_triangle;
extern Bad *motor1;
extern Bad *motor2;
extern Bad *motor3;
extern Bad *horizontal;
extern Bad *base;
extern Bad *bottom_plate;
extern Bad *left1;
extern Bad *rigth1;
extern Bad *palanca2;
extern Bad *sphere;
extern Bad *secondtriangle;
extern Bad *SG90;
extern Bad *SG90holder;
extern Bad *plane;
extern Bad *esfera;


extern int posx;
extern int pos2;
extern int cameraposZ;
extern float degreeZ;
extern bool joystick_found;
extern bool rtopview;
std::string source;

bool si = false;
bool fin = false;
FILE *outfile;



void MainWindow::execute(){
    /*yMoverU();
    zMoverU();
    qApp->processEvents();*/
}


static PyObject* foo(PyObject* self, PyObject* args){
    //yPOS--;
    float x,y,z;
    float k;

    if(PyArg_ParseTuple(args, "fff",&x,&y,&z))
    {
        printf("C++: uarm.gears(%f,%f,%f)\n",x,y,z);
        qDebug()<<" uarms";
        xPOS=x;
        yPOS=y;
        zPOS=z;


    }
    return PyLong_FromLong(0);

    QString s= QString::number((float)yPOS);
    std::cout << "In yMoverU "<<std::endl;
    s="pos Y "+s+"\r";

    small_gear2->degree=yPOS*ratio;
    Gear2->degree=-yPOS;
    horizontal->degree=Gear2->degree;//link3 y link004
    palanca2->degree=Gear2->degree;//link2 y link_3
    arm1->degree=horizontal->degree;//link6
    arm2->deltaZ=-65*sin(Gear2->degree*2*3.1416/360);//link7
    arm2->deltaY=65-65*cos(Gear2->degree*2*3.1416/360);//link7
    secondtriangle->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;//link005,link5
    secondtriangle->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;//link005,link5
    SG90->deltaY=secondtriangle->deltaY;//Dj9g
    SG90->deltaZ=secondtriangle->deltaZ;//Dj9g
    SG90holder->deltaY=secondtriangle->deltaY;//board1
    SG90holder->deltaZ=secondtriangle->deltaZ;//board1

    link3->degree=Gear2->degree;//link3 y link004
    link004->degree=Gear2->degree;//link3 y link004
    link2->degree=Gear2->degree;//link2 y link_3
    link3->degree=Gear2->degree;//link2 y link_3
    link6->degree=horizontal->degree;//link6
    link7->deltaZ=-65*sin(Gear2->degree*2*3.1416/360);//link7
    link7->deltaY=65-65*cos(Gear2->degree*2*3.1416/360);//link7
    link005->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;//link005,link5
    link5->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;//link005,link5
    link005->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;//link005,link5
    link5->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;//link005,link5
    Dj9g->deltaY=secondtriangle->deltaY;//Dj9g
    Dj9g->deltaZ=secondtriangle->deltaZ;//Dj9g
    board1->deltaY=secondtriangle->deltaY;//board1
    board1->deltaZ=secondtriangle->deltaZ;//board1

    if(serial->isOpen()){
        serial->write(s.toStdString().c_str());
        serial->flush();
    }

    //ui->plainTextEdit->insertPlainText(s);
    //ui->lcdL->display((double)(yPOS));
    qApp->processEvents();
    return PyLong_FromLong(0);
};



static PyObject* uarm_show(PyObject* self, PyObject* args)
{
    PyObject *a;
    if(PyArg_UnpackTuple(args, "", 1, 1, &a))
    {
        printf("C++: show(%ld)\n", PyLong_AsLong(a));
    }
    return PyLong_FromLong(0);
}

static PyObject* uarm_add(PyObject* self,PyObject* args)
{
    PyObject *a,*b,*c,*d;
    char *s;
    float x,y,z;

    s=(char *)malloc(12);
    if(PyArg_ParseTuple(args, "sfff",&s,&x,&y,&z))
    {
        printf("C++: a %s add(%f,%f,%f)\n",s,x,y,z);
        //esfera=new Bad();
        //esfera->loader(s,GREEN,x,y,z);
    }
    return PyLong_FromLong(0);

}
 extern "C" PyObject* MainWindow::uarm_gears(PyObject* self,PyObject* args)
    {
        float x,y,z;
        MainWindow *a;

        if(PyArg_ParseTuple(args, "fff",&x,&y,&z))
        {
            printf("C++: uarm.gears(%f,%f,%f)\n",x,y,z);
            qDebug()<<" uarms?";
            xPOS=x-1;
            yPOS=y-1;
            zPOS=z-1;
            a->xMoverU();
            a->yMoverU();
            a->zMoverU();
            execute();
            qApp->processEvents();

        }
        return PyLong_FromLong(0);
    }

static struct PyMethodDef methods[] = {
    { "foo", (PyCFunction )MainWindow::uarm_foo, METH_VARARGS, "Returns the number"},
    { "show", uarm_show, METH_VARARGS, "Show a number" },
    { "add", uarm_add, METH_VARARGS, "Add an object" },
    { "gears",(PyCFunction )MainWindow::uarm_gears, METH_VARARGS, "Move the gears certain angle" },
    { "calculate",(PyCFunction) MainWindow::Kalculate,METH_VARARGS,"Same as calculate but in python"},
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef modDef = {
    PyModuleDef_HEAD_INIT, "uarm", NULL, -1, methods,
    NULL, NULL, NULL, NULL
};

 static  PyObject* PyInit_uarm(void)
{
    return PyModule_Create(&modDef);
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    loadFile("position.pos",source);
    joy = new joystick();
    joy->start();
    ui->LED->setText(joy->name);
    serial= new QSerialPort(this);
    ui->lcdNumberX->setPalette(Qt::blue);
    ui->lcdNumberY->setPalette(Qt::blue);
    ui->lcdNumberZ->setPalette(Qt::blue);
    str=ui->SPort->text();
    ui->espera->setText("150");
    serial->setPortName(str);
    serial->setBaudRate(QSerialPort::Baud57600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    //serial->setFlow(QSerialPort::NoFlow);
    serial->open(QIODevice::ReadWrite);
    //serial->write("ok*");

    connect(serial,SIGNAL(readyRead()),this,SLOT(serialreceived()));
    connect(ui->Enabled_B,SIGNAL(pressed()),this,SLOT(serialM17()));
    connect(ui->Disabled_B,SIGNAL(pressed()),this,SLOT(serialM18()));
    connect(ui->SetZero,SIGNAL(pressed()),this,SLOT(setzero()));
    connect(ui->xmover, SIGNAL(valueChanged(int)),this, SLOT(xMover(int)));
    connect(ui->ymover, SIGNAL(valueChanged(int)),this, SLOT(yMover(int)));
    connect(ui->zmover, SIGNAL(valueChanged(int)),this, SLOT(zMover(int)));

    connect(ui->UP, SIGNAL(pressed()),this, SLOT(yMoverU( )));
    connect(ui->DOWN, SIGNAL(pressed()),this, SLOT(yMoverD( )));

    connect(ui->RIGHT, SIGNAL(pressed()),this, SLOT(zMoverU( )));
    connect(ui->LEFT, SIGNAL(pressed()),this, SLOT(zMoverD( )));

    connect(ui->FRONT, SIGNAL(pressed()),this, SLOT(xMoverU( )));
    connect(ui->BACK, SIGNAL(pressed()),this, SLOT(xMoverD( )));
    connect(ui->feedback, SIGNAL(pressed()),this, SLOT(feedbacks( )));
    connect(ui->Calculate, SIGNAL(pressed()),this, SLOT(calculate( )));
    connect(ui->readfile, SIGNAL(pressed()),this, SLOT(Mover( )));

    connect(ui->grip, SIGNAL(valueChanged(int)),this, SLOT(Abrir_grip( int)));
    connect(ui->wrist, SIGNAL(valueChanged(int)),this, SLOT(Girar_writs(int  )));

    connect(ui->CONNECT, SIGNAL(pressed()),this, SLOT(konnect( )));//Connect to a serial port

    //connect(joy, SIGNAL(up()),this, SLOT(Arriba( )));
    //connect(joy, SIGNAL(down()),this, SLOT(Abajo( )));
    /*connect(joy, SIGNAL(right()),this, SLOT(zMoverU( )));
    connect(joy, SIGNAL(left()),this, SLOT(zMoverD( )));
    connect(joy, SIGNAL(front()),this, SLOT(xMoverU( )));//Front
    connect(joy, SIGNAL(back()),this, SLOT(xMoverD( )));//Back
*/
    connect(ui->PYTHON, SIGNAL(pressed()),this, SLOT(python( )));//Back


    //Girar_wrist(0);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateg()));
    timer->start(100);



    plane->loader("plane.stl",WHITE,-15.69123,9.69769,-113.6832);//done
    Gear2->loader("main_gear2.stl",WHITE,-6.27026,26.1353,-24.29232);//done
    main_gear->loader("main_gear1.stl",GREEN,21.32011,26.63316,-24.63571);//done
    main_gear3->loader("main_gear3.stl",RED,0.1657748,-13.9068,-87.19798);//done
    arm1->loader("arm1.stl",GREEN,13.17923,64.80094,121.3165);//done
    arm2->loader("arm2.stl",GREEN,-6.9115,-37.98584,-24.18324);//done
    arm3->loader("arm3.stl",RED,30.40645,-25.20684,2.76064);//done
    palanca2->loader("palanca2.stl",WHITE,-12.51398,26.9935,-24.56469);//

    main_bar->loader("main_bar.stl",GREEN,0.5358276,26.40598,-24.07585);//done
    small_gear1->loader("small_gear1.stl",GREEN,12.82928,-12.99233,-45.99202);//done
    small_gear2->loader("small_gear2.stl",YELLOW,-5.84519,-11.63362,-45.63098 );//done
    small_gear3->loader("small_gear3.stl",RED,0.0344124,-61.65918 ,-78.58023);//done
    upper_triangle->loader("upper_triangle.stl",GREEN,0.5358181,24.30385,95.92947);//done
    motor1->loader("motor1.stl",BLACK,47.34509,-12.99106,-45.59805);//done
    motor2->loader("motor2.stl",BLACK,-46.27036,-10.68593,-46.34775);//done
    motor3->loader("motor3.stl",BLACK,0.4358006,-61.57526,-70.05495);//done
    horizontal->loader("horizontal.stl",GREEN,7.61905,24.53701,95.57565);//done
    base->loader("base.stl",GREEN,-0.328019,-16.33702,-58.00608);//done
    bottom_plate->loader("bottom.stl",RED,-13.79333,33.5328,3.79485);//done
    left1->loader("left.stl",RED,-30.85771,26.58718,15.50329);//done
    rigth1->loader("rigth.stl",RED,21.32011,26.58718,15.50329 );//done
    sphere->loader("sphere.stl",RED,0,1,0);
    secondtriangle->loader("secondtriangle.stl",GREEN,14.91295,143.89566,93.21524);//done

    SG90->loader("SG90.stl",BLUE,25.16291,186.04488,72.00875);//done
    SG90holder->loader("SG90holder.stl",YELLOW,19.18367,211.53325,93.99998);//done

    //////////////////from here on tthe uarm files

    base01->loader("base01.stl",YELLOW,0,0,-113.68);//done
    base02->loader("base02.stl",YELLOW,0,0,-113.68);//done
    base03->loader("base03.stl",YELLOW,0,0,-113.68);//done
    base05->loader("base05.stl",YELLOW,0,0,0);//done
    base06->loader("base06.stl",YELLOW,0,0,0);//done
    base07->loader("base07.stl",YELLOW,0,0,0);//done
    base08->loader("base08.stl",YELLOW,0,0,0);//done
    base09->loader("base09.stl",YELLOW,0,0,0);//done
    base10->loader("base10.stl",YELLOW,0,0,0);//done
    base11->loader("base11.stl",YELLOW,0,0,0);//done
    base12->loader("base12.stl",YELLOW,0,0,0);//done
    base13->loader("base13.stl",YELLOW,0,0,0);//done
    base14->loader("base14.stl",YELLOW,0,0,0);//done
    bearing->loader("bearing.stl",YELLOW,0,0,0);//done
    bearing_f5x9x012->loader("bearing_f5x9x012.stl",YELLOW,0,0,0);//done
    board1->loader("board1.stl",YELLOW,0,0,0);//done
    bolt->loader("bolt.stl",YELLOW,0,0,0);//done
    bolt22->loader("bolt22.stl",YELLOW,0,0,0);//done
    bolt23->loader("bolt23.stl",YELLOW,0,0,0);//done
    bolt27->loader("bolt27.stl",YELLOW,0,0,-113.680);//done
    bolt39->loader("bolt39.stl",YELLOW,0,0,-113.680);//done
    DJ001->loader("DJ001.stl",BLACK,0,0,-113.680);//done
    DJ002->loader("DJ002.stl",BLACK,0,0,-113.680);//done
    Dj9g->loader("Dj9g.stl",BLACK,0,0,0);//done
    DP->loader("DP.stl",BLACK,0,0,0);//done
    DP001->loader("DP001.stl",BLACK,0,0,0);//done
    DP002->loader("DP002.stl",BLACK,0,0,0);//done
    link1->loader("link1.stl",YELLOW,0,0,0);//done
    link2->loader("link2.stl",YELLOW,0,0,0);//done
    link_3->loader("link003.stl",YELLOW,0,0,0);//done
    link3->loader("link3.stl",YELLOW,0,0,0);//done
    link004->loader("link004.stl",YELLOW,0,0,0);//done
    link4->loader("link4.stl",YELLOW,0,0,0);//done
    link005->loader("link005.stl",YELLOW,0,0,0);//done
    link5->loader("link5.stl",YELLOW,0,0,0);//done
    link006->loader("link006.stl",YELLOW,0,0,0);//done
    link6->loader("link6.stl",YELLOW,0,0,0);//done
    link7->loader("link7.stl",YELLOW,0,0,0);//done
    link8->loader("link8.stl",YELLOW,0,0,0);//done
    s1->loader("s1.stl",YELLOW,0,0,0);//done
    s002->loader("s002.stl",YELLOW,0,0,0);//done
    s2->loader("s2.stl",YELLOW,0,0,0);//done
    STM4_22->loader("STM4_22.stl",YELLOW,0,0,0);//done

    //////////////////////////
    // mover(source);
}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
}

int MainWindow::serialreceived()
{

    QByteArray data = serial->readAll();
    const char *posi;
    const char *kl;
    int d;
    // parse((char *)data.constData());
    // printf("command %s %s %s %s %s %s\n",argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]);
    printf("%s",(char *)data.constData());


     if(strcmp(argv[0],"L")==0)
     {
        printf("command %s %s %s %s %s %s\n",argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]);
        d=atoi(argv[2]);
        xPOS=(d-789)/1.9272;
        xMoverU();

        d=atoi(argv[5]);
        yPOS=(d-789+200)/1.9272;
        yMoverU();
        return 1;
     }

    if(strcmp((char *)data.constData(),"ok")==0)
    {
        si=true;
        printf("si");
        return 1;
     }

     if(strcmp((char *)data.constData(),"fin")==0)
     {
      fin=true;
      printf("fin");
      return 1;
     }

     if(strcmp(argv[0],"nok")==0)
     {
      si=false;
      return 0;
     }
}


int MainWindow::received()
{
    QByteArray data = serial->readAll();

    //parse((char *)data.constData());
    printf("[%s]",(char *)data.constData());
    if(strcmp((char *)data.constData(),"ok")==0)
    {
      printf("okj");
      return 1;
    }
    return 0;

}


void MainWindow::serialM17()
{
    if(serial->isOpen()){
    serial->write("M17\r");
    }
}

void MainWindow::serialM18()
{
    if(serial->isOpen()){
    serial->write("M18\r");
    }
}

void MainWindow::setzero()
{
    serial->write("reset\r");
}


void MainWindow::xMover(int x)
{
    QString s= QString::number((float)x/1);
    posx=(float) x;
    s="pos X "+s+"\r";
   // serial->write(s.toStdString().c_str());
    ui->lcdNumberX->display((double)x/10);
}

void MainWindow::yMover(int y) // Moves away
{

    pos2=(float)y;
    ui->lcdNumberY->display((double)(y));

}

void MainWindow::zMover(int z)//Camera Z pos
{
    cameraposZ=(float)z;
    ui->lcdNumberZ->display((double)(z));
}


void MainWindow::yMoverU() //UP
{
    //yPOS--;
    float k;
    QString s= QString::number((float)yPOS);
    std::cout << "In yMoverU "<<std::endl;
    s="pos Y "+s+"\r";

    small_gear2->degree=yPOS*ratio;
    Gear2->degree=-yPOS;
    horizontal->degree=Gear2->degree;//link3 y link004
    palanca2->degree=Gear2->degree;//link2 y link_3
    arm1->degree=horizontal->degree;//link6
    arm2->deltaZ=-65*sin(Gear2->degree*2*3.1416/360);//link7
    arm2->deltaY=65-65*cos(Gear2->degree*2*3.1416/360);//link7
    secondtriangle->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;//link005,link5
    secondtriangle->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;//link005,link5
    SG90->deltaY=secondtriangle->deltaY;//Dj9g
    SG90->deltaZ=secondtriangle->deltaZ;//Dj9g
    SG90holder->deltaY=secondtriangle->deltaY;//board1
    SG90holder->deltaZ=secondtriangle->deltaZ;//board1



        link3->degree=Gear2->degree;//link3 y link004
        link004->degree=Gear2->degree;//link3 y link004
        link2->degree=Gear2->degree;//link2 y link_3
        link3->degree=Gear2->degree;//link2 y link_3
        link6->degree=horizontal->degree;//link6
        link7->deltaZ=-65*sin(Gear2->degree*2*3.1416/360);//link7
        link7->deltaY=65-65*cos(Gear2->degree*2*3.1416/360);//link7
        link005->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;//link005,link5
        link5->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;//link005,link5
        link005->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;//link005,link5
        link5->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;//link005,link5
        Dj9g->deltaY=secondtriangle->deltaY;//Dj9g
        Dj9g->deltaZ=secondtriangle->deltaZ;//Dj9g
        board1->deltaY=secondtriangle->deltaY;//board1
        board1->deltaZ=secondtriangle->deltaZ;//board1


    if(serial->isOpen()){
        serial->write(s.toStdString().c_str());
        serial->flush();
        }

    //ui->plainTextEdit->insertPlainText(s);
    //ui->lcdL->display((double)(yPOS));
}

void MainWindow::yMoverD() //DOWN
{
    //yPOS++;
    QString s= QString::number((float)yPOS);
   // std::cout << "In xMOverD "<<std::endl;

    s="pos Y "+s+"\r";
    small_gear2->degree=yPOS*ratio;
    Gear2->degree=-yPOS;
    horizontal->degree=Gear2->degree;
    palanca2->degree=Gear2->degree;
    arm1->degree=horizontal->degree;
    arm2->deltaZ=-65*sin(Gear2->degree*2*3.1416/360);
    arm2->deltaY=65-65*cos(Gear2->degree*2*3.1416/360);
    secondtriangle->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;
    secondtriangle->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;
    SG90->deltaY=secondtriangle->deltaY;
    SG90->deltaZ=secondtriangle->deltaZ;
    SG90holder->deltaY=secondtriangle->deltaY;
    SG90holder->deltaZ=secondtriangle->deltaZ;

    link3->degree=Gear2->degree;//link3 y link004
    link004->degree=Gear2->degree;//link3 y link004
    link2->degree=Gear2->degree;//link2 y link_3
    link3->degree=Gear2->degree;//link2 y link_3
    link6->degree=horizontal->degree;//link6
    link7->deltaZ=-65*sin(Gear2->degree*2*3.1416/360);//link7
    link7->deltaY=65-65*cos(Gear2->degree*2*3.1416/360);//link7
    link005->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;//link005,link5
    link5->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;//link005,link5
    link005->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;//link005,link5
    link5->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;//link005,link5
    Dj9g->deltaY=secondtriangle->deltaY;//Dj9g
    Dj9g->deltaZ=secondtriangle->deltaZ;//Dj9g
    board1->deltaY=secondtriangle->deltaY;//board1
    board1->deltaZ=secondtriangle->deltaZ;//board

    if(serial->isOpen()){
        serial->write(s.toStdString().c_str());
        serial->waitForBytesWritten(-1);
        serial->flush();
    }

    ui->plainTextEdit->insertPlainText(s);
    ui->lcdL->display((double)(yPOS));
}

void MainWindow::xMoverU()  //FRONT
{
    //xPOS=xPOS-1;
    float k;
    QString s= QString::number((float)xPOS);
    std::cout << "In xMOverU "<<std::endl;

    s="pos X "+s+"\r";
    small_gear1->degree=-xPOS*ratio;
    main_gear->degree=xPOS;
    main_bar->degree=main_gear->degree;
    arm3->degree=main_bar->degree;

    upper_triangle->deltaY=-120*sin( main_bar->degree*2*3.1416/360);
    upper_triangle->deltaZ=-120+120*cos( main_bar->degree*2*3.1416/360);
    horizontal->deltaY=-120*sin( main_bar->degree*2*3.1416/360);
    horizontal->deltaZ=-120+120*cos( main_bar->degree*2*3.1416/360);

    arm2->degree=arm3->degree;

    arm1->deltaY=horizontal->deltaY;
    arm1->deltaZ=horizontal->deltaZ;

    secondtriangle->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;
    secondtriangle->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;

    SG90->deltaY=secondtriangle->deltaY;
    SG90->deltaZ=secondtriangle->deltaZ;
    SG90holder->deltaY=secondtriangle->deltaY;
    SG90holder->deltaZ=secondtriangle->deltaZ;

    if(serial->isOpen()){
        serial->write(s.toStdString().c_str());
        serial->waitForBytesWritten(-1);
        serial->flush();
    }

    //ui->plainTextEdit->insertPlainText(s);
    //ui->lcdR->display((double)(xPOS));
}

void MainWindow::xMoverD() //BACK
{
    //xPOS=xPOS+1;
    QString s= QString::number((float)xPOS/1);
    //std::cout << "In xMOverD "<<std::endl;
    s="pos X "+s+"\r";
    small_gear1->degree=-xPOS*ratio;
    main_gear->degree=xPOS;
    main_bar->degree=main_gear->degree;
    arm3->degree=main_bar->degree;

    upper_triangle->deltaY=-120*sin( main_bar->degree*2*3.1416/360);
    upper_triangle->deltaZ=-120+120*cos( main_bar->degree*2*3.1416/360);
    horizontal->deltaY=-120*sin( main_bar->degree*2*3.1416/360);
    horizontal->deltaZ=-120+120*cos( main_bar->degree*2*3.1416/360);
    arm2->degree=arm3->degree;
    horizontal->deltaY=-120*sin( main_bar->degree*2*3.1416/360);
    horizontal->deltaZ=-120+120*cos( main_bar->degree*2*3.1416/360);
    arm1->deltaY=horizontal->deltaY;
    arm1->deltaZ=horizontal->deltaZ;
    secondtriangle->deltaY=-120+120*cos(horizontal->degree*3.1416/180)+horizontal->deltaY;
    secondtriangle->deltaZ=120*sin(horizontal->degree*3.1416/180)+horizontal->deltaZ;
    SG90->deltaY=secondtriangle->deltaY;
    SG90->deltaZ=secondtriangle->deltaZ;
    SG90holder->deltaY=secondtriangle->deltaY;
    SG90holder->deltaZ=secondtriangle->deltaZ;

    if(serial->isOpen()){
        serial->write(s.toStdString().c_str());
        serial->waitForBytesWritten(-1);
        }

    ui->plainTextEdit->insertPlainText(s);
    ui->lcdR->display((double)(xPOS));
}

void MainWindow::zMoverU()
{
   // zPOS=zPOS-1;
    degreeZ=zPOS;
    QString s= QString::number((float)zPOS/1);
    s="pos Z "+s+"\r";

    if(serial->isOpen()){
        serial->write(s.toStdString().c_str());
        }
    //ui->lcdNumberY->display((double)(zPOS/10));

    small_gear2->degreeZ=degreeZ*ratio;
    Gear2->degreeZ=degreeZ;
    horizontal->degreeZ=degreeZ;
    palanca2->degreeZ=degreeZ;
    arm1->degreeZ=degreeZ;
    arm2->degreeZ=degreeZ;
    arm3->degreeZ=degreeZ;
    left1->degreeZ=degreeZ;
    rigth1->degreeZ=degreeZ;
    small_gear1->degreeZ=degreeZ;
    small_gear3->degreeZ=degreeZ;
    motor1->degreeZ=motor2->degreeZ=motor3->degreeZ=degreeZ;
    palanca2->degreeZ=degreeZ;
    bottom_plate->degreeZ=degreeZ;
    main_bar->degreeZ=secondtriangle->degreeZ=degreeZ;
    main_gear->degreeZ=upper_triangle->degreeZ=degreeZ;
    SG90->degreeZ=upper_triangle->degreeZ=degreeZ;
    SG90holder->degreeZ=upper_triangle->degreeZ=degreeZ;
    //ui->lcdZ->display((double)(zPOS));
}

void MainWindow::zMoverD() //Moves the object around Z
{
    //zPOS=zPOS+1;
    degreeZ=zPOS;
    QString s= QString::number((float)zPOS);
    s="pos Z "+s+"\r";
    if(serial->isOpen()){
       serial->write(s.toStdString().c_str());
       }
    ui->lcdNumberY->display((double)(zPOS));

    small_gear2->degreeZ=degreeZ;
    Gear2->degreeZ=degreeZ;;
    horizontal->degreeZ=degreeZ;
    palanca2->degreeZ=degreeZ;
    arm1->degreeZ=degreeZ;
    arm2->degreeZ=degreeZ;
    arm3->degreeZ=degreeZ;
    left1->degreeZ=degreeZ;
    rigth1->degreeZ=degreeZ;
    small_gear1->degreeZ=degreeZ;
    small_gear3->degreeZ=degreeZ;
    motor1->degreeZ=motor2->degreeZ=motor3->degreeZ=degreeZ;
    palanca2->degreeZ=degreeZ;
    bottom_plate->degreeZ=degreeZ;
    main_bar->degreeZ=secondtriangle->degreeZ=degreeZ;
    main_gear->degreeZ=upper_triangle->degreeZ=degreeZ;

    SG90->degreeZ=upper_triangle->degreeZ=degreeZ;
    SG90holder->degreeZ=upper_triangle->degreeZ=degreeZ;

    ui->lcdZ->display((double)(zPOS));
 }

void MainWindow::updateg()//ler
{
    static float j=0;
    static float h=0;
    static float k=0;
    float delta=0.5;
    float jforward=1.05;
    float jbackward=0.95;
    float X2,Y2;

    QString s;
    s="backl\r";
    if(feedbackb==true)
        serial->write(s.toStdString().c_str());
    //ui->lcdNumberY->display((double)(zPOS/10));
    timer->start(100);
    if(UJ==1){ //Up only moves the Z up
        Z1+=delta;
        qDebug()<<"UP"<<j;
        moving(X1,Y1,Z1+j);
        }
    if(DJ==1){ //Up only moves the Z down
        Z1-=delta;
        qDebug()<<"DOWN"<<j;
        moving(X1,Y1,Z1-j);
        }
    if(RJ==1){
        gamma=3.1416*5/180;
        X2=X1*cos(gamma)-Y1*sin(gamma);
        Y2=X1*sin(gamma)+Y1*cos(gamma);
        X1=X2;
        Y1=Y2;
        qDebug()<<"Gamma+ "<<gamma*180/3.1416;

        moving(X1,Y1-h,Z1);
        }
    if(LJ==1){
        gamma=-3.1416*5/180;
        X2=X1*cos(gamma)-Y1*sin(gamma);
        Y2=X1*sin(gamma)+Y1*cos(gamma);
        X1=X2;
        Y1=Y2;
        qDebug()<<"Gamma- "<<gamma*180/3.1416;
        moving(X1,Y1+h,Z1);
        }
    if(FJ==1){  //Get X1 and Y1 and the angle. Then calculate the line and increase it
        X1=X1*jforward;
        Y1=Y1*jforward;
        qDebug()<<"RIGHT "<<h;
        moving(X1,Y1-h,Z1);
        }
    if(BJ==1){//Get X1 and Y1 and the angle. Then calculate the line and increase it
        X1=X1*jbackward;
        Y1=Y1*jbackward;
        qDebug()<<"LEFT "<<h;
        moving(X1,Y1,Z1);
        }
}


void MainWindow::updateg2()
{

    QString s;
    s="backr\r";
    if(serial->isOpen()){
    serial->write(s.toStdString().c_str());
    }
    //ui->lcdNumberY->display((double)(zPOS/10));


}


void MainWindow::feedbacks()// This was diosabled in darm
{
    feedbackb=!feedbackb;
}

void MainWindow::calculate()
{
   float x,y,z,alpha,beta,gamma,m,l,c,n,phi,s,theta;
   QString str;
   l=12;
   a1=0;b1=12;c1=12;

   float x0,y0,z0,a1,b1,c1,t,x1,y1,z1,m1;//http://tutorial.math.lamar.edu/Classes/CalcIII/EqnsOfLines.aspx

   x0=(float) X1;
   y0=(float) Y1;
   z0=(float) Z1;

   str=ui->X->toPlainText();
   x=str.toDouble();

   str=ui->Y->toPlainText();
   y=str.toDouble();

   str=ui->Z->toPlainText();
   z=str.toDouble();

   a1=x0-x;
   b1=y0-y;
   c1=z0-z;

   t=(x-x0)/a1;//parametrizaion

   qDebug()<<"a= "<<a1;
   qDebug()<<"b= "<<b1;
   qDebug()<<"c= "<<c1;
   qDebug()<<"t= "<<t;

   if(t>0){
       for(m1=0;m1<1.01;m1=m1+0.01)
       {
           x=x0+a1*m1;
           y=y0+b1*m1;
           z=z0+c1*m1;
           qDebug()<<"x= "<<x<<" y= "<<y<<" z= "<<z<<"m1= "<<m1;

            X1=x;
            Y1=y;
            Z1=z;

           s=sqrt(x*x+y*y);
           theta=atan(y/x);
           //// Then change y by s
           m=sqrt(s*s+z*z);

           alpha=acos(m/(2*l));
           gamma=atan(z/s);
           beta=3.1416/2-alpha-gamma;
           c=3.1416/2-beta;
           phi=acos((m*m-2*l*l)/(-2*l*l));
           n=-(3.1416-c-phi);
           QString BETA = QString::number(beta*180/3.1416);
           QString ALPHA = QString::number(n*180/3.1416);
           QString GAMMA = QString::number(gamma*180/3.1416);
           QString PHI = QString::number(phi*180/3.1416);
           QString THETA =QString::number(90-theta*180/3.1416);
           QString S =QString::number(s);
           QString M = QString::number(m);
           ui->alpha->setText(ALPHA);
           ui->beta->setText(BETA);
           ui->gamma->setText(GAMMA);
           ui->theta->setText(THETA);
           ui->m->setText(M);
           ui->s->setText(S);
           ui->phi->setText(PHI);
           xPOS=-beta*180/(3.1416);
           yPOS=-n*180/(3.1416);
           zPOS=90-theta*180/3.1416;
           qDebug()<<"m= "<<m;
           xMoverD();
           yMoverD();
           zMoverD();

           //ui->lcdNumberY->display((double)(xPOS));

       }
   }
   else
   {
       qDebug()<<"Negative";
       for(m1=0;m1>-1.01;m1=m1-0.01)
       {
           x=x0+a1*m1;
           y=y0+b1*m1;
           z=z0+c1*m1;
           qDebug()<<"x= "<<x<<" y= "<<y<<" z= "<<z<<"m1= "<<m1;
          // QThread::msleep(5);

           X1=x;
           Y1=y;
           Z1=z;

           s=sqrt(x*x+y*y);
           theta=atan(y/x);
           //// Then change y by s
           m=sqrt(s*s+z*z);

           alpha=acos(m/(2*l));
           gamma=atan(z/s);
           beta=3.1416/2-alpha-gamma;
           c=3.1416/2-beta;
           phi=acos((m*m-2*l*l)/(-2*l*l));
           n=-(3.1416-c-phi);
           QString BETA = QString::number(beta*180/3.1416);
           QString ALPHA = QString::number(n*180/3.1416);
           QString GAMMA = QString::number(gamma*180/3.1416);
           QString PHI = QString::number(phi*180/3.1416);
           QString THETA =QString::number(90-theta*180/3.1416);
           QString S =QString::number(s);
           QString M = QString::number(m);
           ui->alpha->setText(ALPHA);
           ui->beta->setText(BETA);
           ui->gamma->setText(GAMMA);
           ui->theta->setText(THETA);
           ui->m->setText(M);
           ui->s->setText(S);
           ui->phi->setText(PHI);
           xPOS=-beta*180/(3.1416);
           yPOS=-n*180/(3.1416);
           zPOS=90-theta*180/3.1416;
           //qDebug()<<"m= "<<m;
           xMoverD();
           yMoverD();
           zMoverD();
            qApp->processEvents();
           //ui->lcdNumberY->display((double)(xPOS));

       }

   }

}

extern "C" PyObject* MainWindow::Kalculate(PyObject* self,PyObject* args)
{
    float x2,y2,z2;
    MainWindow *a;

    if(PyArg_ParseTuple(args, "fff",&x2,&y2,&z2))
    {
        printf("C++: uarm.Kalculate(%f,%f,%f)\n",x2,y2,z2);
        qDebug()<<" Kalculate";
    }

   float x,y,z,alpha,beta,gamma,m,l,c,n,phi,s,theta;
   QString str;
   l=12;
   a1=0;b1=12;c1=12;

   float x0,y0,z0,a1,b1,c1,t,x1,y1,z1,m1;//http://tutorial.math.lamar.edu/Classes/CalcIII/EqnsOfLines.aspx


   x0=(float) X1;
   y0=(float) Y1;
   z0=(float) Z1;


   x=x2;
   y=y2;
   z=z2;

   a1=x0-x;
   b1=y0-y;
   c1=z0-z;

   t=(x-x0)/a1;//parametrizaion

   qDebug()<<"a= "<<a1;
   qDebug()<<"b= "<<b1;
   qDebug()<<"c= "<<c1;
   qDebug()<<"t= "<<t;

   if(t>0){
       for(m1=0;m1<1.01;m1=m1+0.01)
       {
           x=x0+a1*m1;
           y=y0+b1*m1;
           z=z0+c1*m1;
           qDebug()<<"x= "<<x<<" y= "<<y<<" z= "<<z<<"m1= "<<m1;

            X1=x;
            Y1=y;
            Z1=z;

           s=sqrt(x*x+y*y);
           theta=atan(y/x);
           //// Then change y by s
           m=sqrt(s*s+z*z);

           alpha=acos(m/(2*l));
           gamma=atan(z/s);
           beta=3.1416/2-alpha-gamma;
           c=3.1416/2-beta;
           phi=acos((m*m-2*l*l)/(-2*l*l));
           n=-(3.1416-c-phi);
           xPOS=-beta*180/(3.1416);
           yPOS=-n*180/(3.1416);
           zPOS=90-theta*180/3.1416;
           qDebug()<<"m= "<<m;
          // QThread::msleep (100);
           a->xMoverU();
           a->yMoverU();
           a->zMoverU();

           qApp->processEvents();

           //ui->lcdNumberY->display((double)(xPOS));

       }
   }
   else
   {
       qDebug()<<"Negative";
       for(m1=0;m1>-1.01;m1=m1-0.01)
       {
           x=x0+a1*m1;
           y=y0+b1*m1;
           z=z0+c1*m1;
           qDebug()<<"x= "<<x<<" y= "<<y<<" z= "<<z<<"m1= "<<m1;
          // QThread::msleep(5);

           X1=x;
           Y1=y;
           Z1=z;

           s=sqrt(x*x+y*y);
           theta=atan(y/x);
           //// Then change y by s
           m=sqrt(s*s+z*z);

           alpha=acos(m/(2*l));
           gamma=atan(z/s);
           beta=3.1416/2-alpha-gamma;
           c=3.1416/2-beta;
           phi=acos((m*m-2*l*l)/(-2*l*l));
           n=-(3.1416-c-phi);
           xPOS=-beta*180/(3.1416);
           yPOS=-n*180/(3.1416);
           zPOS=90-theta*180/3.1416;
           //qDebug()<<"m= "<<m;
            //QThread::msleep (100);
           a->xMoverU();
           a->yMoverU();
           a->zMoverU();

           qApp->processEvents();

           //ui->lcdNumberY->display((double)(xPOS));

       }

   }

   return PyLong_FromLong(0);
}


void MainWindow::actuator(float a,float b,float c1)
{
   float x,y,y1,z,alpha,beta,gamma,m,l,c,n,phi,s,theta;
   QString str;
   std::cout << "In Actuator: a= "<<a<<" b= "<<b<<" c= "<<c1<<std::endl;
   l=12;
   x=a;
   y=b;
   z=c1;
   s=sqrt(x*x+y*y);
   theta=atan(y/x);
   //// Then change y by s
   m=sqrt(s*s+z*z);
   alpha=acos(m/(2*l));
   gamma=atan(z/s);
   beta=3.1416/2-alpha-gamma;
   c=3.1416/2-beta;
   phi=acos((m*m-2*l*l)/(-2*l*l));
   n=-(3.1416-c-phi);
   QString BETA = QString::number(beta*180/3.1416);
   QString ALPHA = QString::number(n*180/3.1416);
   QString GAMMA = QString::number(gamma*180/3.1416);
   QString PHI = QString::number(phi*180/3.1416);
   QString THETA =QString::number(90-theta*180/3.1416);
   QString S =QString::number(s);
   QString M = QString::number(m);
   ui->alpha->setText(ALPHA);
   ui->beta->setText(BETA);
   ui->gamma->setText(GAMMA);
   ui->theta->setText(THETA);
   ui->m->setText(M);
   ui->s->setText(S);
   ui->phi->setText(PHI);
   xPOS=-beta*180/(3.1416);
   yPOS=-n*180/(3.1416);
   zPOS=90-theta*180/3.1416;
   fprintf(outfile,"left angle[%0.2f], right angle[%0.2f], z angle[%0.2f]\n",xPOS,yPOS,zPOS);
   xMoverU();
   yMoverU();
   zMoverU();
   qApp->processEvents();

}


void MainWindow::parse(char *cmd)
{

    int i=0;
    printf("%s",cmd);
    argv[i] = strtok(cmd, " ");

    do
    {
        argv[++i] = strtok(NULL, " ");
    }
    while ((i < 30) && (argv[i] != NULL));
}

void MainWindow::Mover()
{
    mover(source);
}

void MainWindow::mover(std::string& str)
{
   float a,b,c;
   float DeltaA,DeltaB,DeltaC;
   float deltaA,deltaB,deltaC;
   float maxvel=0.1;
   float iterations;
   int e;
   char s;
   QString sleep;
   char tmp[300];

    outfile=fopen("C:/Users/German/Documents/programacion/test/bin/Release/out.txt","w");

    if(outfile==NULL)
    {
       printf("No out text\n");
    }
    fprintf(outfile,"Starting point: X= %0.2f, Y= %0.2f, Z= %0.2f\n",a1,b1,c1);
    std::ifstream in("C:/Users/German/Documents/programacion/test/bin/Release/position.pos");

    if(!in.is_open())
    {
        std::cout << "The file cannot be opened\n";
        exit(0);
        return;
    }


    do
    {
       in.getline(tmp,300);
       sscanf(tmp,"%s %f %f %f",&s,&a,&b,&c);
       std::cout << s <<" " << a<<" "<< b << " "<<c <<std::endl;
       fprintf(outfile,"\nCommand: %c, X= %f, Y= %f, Z= %f\n",s,a,b,c);
       fprintf(outfile,"a1= %f, b1= %f, c1= %f\n",a1,b1,c1);
       sleep=ui->espera->toPlainText();
       //std::cout << ".." <<std::endl;

       if(strcmp(&s,"P")==0)
        {
           DeltaA=abs((a-a1)/maxvel);
           DeltaB=abs((b-b1)/maxvel);
           DeltaC=abs((c-c1)/maxvel);
           fprintf(outfile,"DeltaA: %f DeltaB: %f Delta C: %f \n\n",DeltaA,DeltaB,DeltaC);

           if(DeltaA>=DeltaB)
           {
             if(DeltaA>=DeltaC)
                 iterations=DeltaA;
             else
                 iterations=(DeltaC);
           }
           else
           {
            if(DeltaB>=DeltaC)
                iterations=(DeltaB);
            else
                iterations=(DeltaC);
           }

           deltaA=((a-a1)/iterations);
           deltaB=((b-b1)/iterations);
           deltaC=((c-c1)/iterations);
           //printf("#########Iterations %f deltaA= %f deltaB=%f deltaC=%f \n",iterations,deltaA,deltaB,deltaC);
           fprintf(outfile,"deltaA: %f deltaB: %f delta C: %f Iterations: %f\n\n",deltaA,deltaB,deltaC,iterations);
        for(e=0;e<iterations;e++)
            {
            a1=a1+deltaA;
            b1=b1+deltaB;
            c1=c1+deltaC;
           // Sleep(sleep.toInt());

            fprintf(outfile,"Iteration[%i]: X= %0.2f, Y= %0.2f, Z= %0.2f\n",e,a1,b1,c1);
            actuator(a1,b1,c1);
            while(fin!=true)
            {
                printf(".");
                QString s;
                s="stat\r";
                if(serial->isOpen()){
                serial->write(s.toStdString().c_str());
                }
                qApp->processEvents();
               };
           // actuator(a1,b1,c1);
            fprintf(outfile,"%0.2f %0.2f %0.2f\n",a1,b1,c1);
            }
        }
       if(strcmp(&s,"W")==0)
       Girar_wrist(a);
       a1=a;
       b1=b;
       c1=c;
       printf("New a1=%f b1= %f c1=%f\n",a1,b1,c1);
       fin=false;
    }while(!in.eof());
    fclose(outfile);
}

void MainWindow::Abrir_grip(int a)
{
    QString s= QString::number((int)a);
    s="pos G "+s+"\r";
    if(serial->isOpen())
        serial->write(s.toStdString().c_str());

}

void MainWindow::Girar_wrist(int a)
{
    QString s= QString::number((int)a);
    s="pos W "+s+"\r";
    if(serial->isOpen())
        serial->write(s.toStdString().c_str());

    ui->plainTextEdit->insertPlainText(s);
}

void MainWindow::konnect()//connect
{
    if(connected==1){
        serial->close();
        ui->CONNECT->setText("Disconnected");
        connected=0;
    }
    else
    {
        serial->setPortName(str);
        serial->setBaudRate(QSerialPort::Baud57600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        //serial->setFlow(QSerialPort::NoFlow);
        serial->open(QIODevice::ReadWrite);
        ui->CONNECT->setText("Connected");
        connected=1;
    }
}

namespace py = pybind11;
void MainWindow::python()
{
    Py_SetPythonHome(L"C:\\Python34");

    kamino->show();
    PyImport_AppendInittab("uarm", &PyInit_uarm);

    CPyInstance hInstance;
    const char pFile[] = "C:\\Users\\German\\Documents\\programacion\\build-test-Desktop_Qt_5_9_1_MinGW_32bit-Debug\\debug\\pyemb6.py";
    FILE* fp = _Py_fopen(pFile, "r");

    if(fp==NULL)
        std::cout<<"file not found";
    else
        PyRun_AnyFile(fp, pFile);
}

int add(int i, int j) {
    return i + j;
}


PYBIND11_PLUGIN(example) {
    py::module m("example", "pybind11 example plugin");
    m.def("add", &add, "A function which adds two numbers");
    return m.ptr();
}

void MainWindow::on_UARM_clicked()
{
    UARM=true;
    DARM=false;
}

void MainWindow::on_DARM_clicked()
{
    UARM=false;
    DARM=true;
}

void MainWindow::on_ORTHO_clicked()
{
    ORTHO=true;
    PROJECTED=false;
}

void MainWindow::on_PROJECTED_clicked()
{
    ORTHO=false;
    PROJECTED=true;
}

void MainWindow::Arriba()
{
    if(X1<15)
    {
        //X1=X1+0.1;
        //qDebug()<<"WWWW";
        calcular(X1+1,0,0);
    }

}

void MainWindow::Abajo()
{
    if(X1>5)
    {
        //X1=X1-0.1;
        calcular(X1-1,0,0);
    }

}


extern "C" PyObject* MainWindow::uarm_foo(PyObject* self, PyObject* args){
    printf("... in C++...: foo() method\n");
    return PyLong_FromLong(51);
}


void MainWindow::calcular(float x, float y , float z){
    float alpha,beta,gamma,m,l,c,n,phi,s,theta;
    QString str;
    l=12;
    a1=0;b1=12;c1=12;

    float x0,y0,z0,a1,b1,c1,t,x1,y1,z1,m1;//http://tutorial.math.lamar.edu/Classes/CalcIII/EqnsOfLines.aspx

    x0=(float) X1;
    y0=(float) Y1;
    z0=(float) Z1;

    a1=x0-x;
    b1=y0-y;
    c1=z0-z;

    t=(x-x0)/a1;//parametrizaion

   /* qDebug()<<"a= "<<a1;
    qDebug()<<"b= "<<b1;
    qDebug()<<"c= "<<c1;
    qDebug()<<"t= "<<t;*/

    if(t>0){
        for(m1=0;m1<1.01;m1=m1+0.01)
        {
            x=x0+a1*m1;
            y=y0+b1*m1;
            z=z0+c1*m1;
            //qDebug()<<"x= "<<x<<" y= "<<y<<" z= "<<z<<"m1= "<<m1;

             X1=x;
             Y1=y;
             Z1=z;

            s=sqrt(x*x+y*y);
            theta=atan(y/x);
            //// Then change y by s
            m=sqrt(s*s+z*z);

            alpha=acos(m/(2*l));
            gamma=atan(z/s);
            beta=3.1416/2-alpha-gamma;
            c=3.1416/2-beta;
            phi=acos((m*m-2*l*l)/(-2*l*l));
            n=-(3.1416-c-phi);
            QString BETA = QString::number(beta*180/3.1416);
            QString ALPHA = QString::number(n*180/3.1416);
            QString GAMMA = QString::number(gamma*180/3.1416);
            QString PHI = QString::number(phi*180/3.1416);
            QString THETA =QString::number(90-theta*180/3.1416);
            QString S =QString::number(s);
            QString M = QString::number(m);
            ui->alpha->setText(ALPHA);
            ui->beta->setText(BETA);
            ui->gamma->setText(GAMMA);
            ui->theta->setText(THETA);
            ui->m->setText(M);
            ui->s->setText(S);
            ui->phi->setText(PHI);
            xPOS=-beta*180/(3.1416);
            yPOS=-n*180/(3.1416);
            zPOS=90-theta*180/3.1416;
            //qDebug()<<"m= "<<m;
            xMoverD();
            yMoverD();
            zMoverD();

            //ui->lcdNumberY->display((double)(xPOS));

        }
    }
    else
    {
       // qDebug()<<"Negative";
        for(m1=0;m1>-1.01;m1=m1-0.01)
        {
            x=x0+a1*m1;
            y=y0+b1*m1;
            z=z0+c1*m1;
           // qDebug()<<"x= "<<x<<" y= "<<y<<" z= "<<z<<"m1= "<<m1;
           // QThread::msleep(5);

            X1=x;
            Y1=y;
            Z1=z;

            s=sqrt(x*x+y*y);
            theta=atan(y/x);
            //// Then change y by s
            m=sqrt(s*s+z*z);

            alpha=acos(m/(2*l));
            gamma=atan(z/s);
            beta=3.1416/2-alpha-gamma;
            c=3.1416/2-beta;
            phi=acos((m*m-2*l*l)/(-2*l*l));
            n=-(3.1416-c-phi);
            QString BETA = QString::number(beta*180/3.1416);
            QString ALPHA = QString::number(n*180/3.1416);
            QString GAMMA = QString::number(gamma*180/3.1416);
            QString PHI = QString::number(phi*180/3.1416);
            QString THETA =QString::number(90-theta*180/3.1416);
            QString S =QString::number(s);
            QString M = QString::number(m);
            ui->alpha->setText(ALPHA);
            ui->beta->setText(BETA);
            ui->gamma->setText(GAMMA);
            ui->theta->setText(THETA);
            ui->m->setText(M);
            ui->s->setText(S);
            ui->phi->setText(PHI);
            xPOS=-beta*180/(3.1416);
            yPOS=-n*180/(3.1416);
            zPOS=90-theta*180/3.1416;
            //qDebug()<<"m= "<<m;
            xMoverD();
            yMoverD();
            zMoverD();
             qApp->processEvents();
            //ui->lcdNumberY->display((double)(xPOS));

        }

    }

}


void MainWindow::moving(float x, float y , float z){

    float alpha,beta,gamma,m,l,c,n,phi,s,theta;
    QString str;
    l=12;
    a1=0;b1=12;c1=12;

    float x0,y0,z0,a1,b1,c1,t,x1,y1,z1,m1;//http://tutorial.math.lamar.edu/Classes/CalcIII/EqnsOfLines.aspx

    x0=(float) X1;
    y0=(float) Y1;
    z0=(float) Z1;

    a1=x0-x;
    b1=y0-y;
    c1=z0-z;

    t=(x-x0)/a1;//parametrizaion

   /* qDebug()<<"a= "<<a1;
    qDebug()<<"b= "<<b1;
    qDebug()<<"c= "<<c1;
    qDebug()<<"t= "<<t;*/
   /* x=x0+a1*m1;
    y=y0+b1*m1;
    z=z0+c1*m1;*/
    //qDebug()<<"x= "<<x<<" y= "<<y<<" z= "<<z<<"m1= "<<m1;

    X1=x;
    Y1=y;
    Z1=z;

    s=sqrt(x*x+y*y);
    theta=atan(y/x);
    //// Then change y by s
    m=sqrt(s*s+z*z);
    alpha=acos(m/(2*l));
    gamma=atan(z/s);
    beta=3.1416/2-alpha-gamma;
    c=3.1416/2-beta;
    phi=acos((m*m-2*l*l)/(-2*l*l));
    n=-(3.1416-c-phi);
    QString BETA = QString::number(beta*180/3.1416);
    QString ALPHA = QString::number(n*180/3.1416);
    QString GAMMA = QString::number(gamma*180/3.1416);
    QString PHI = QString::number(phi*180/3.1416);
    QString THETA =QString::number(90-theta*180/3.1416);
    QString S =QString::number(s);
    QString M = QString::number(m);
    ui->alpha->setText(ALPHA);
    ui->beta->setText(BETA);
    ui->gamma->setText(GAMMA);
    ui->theta->setText(THETA);
    ui->m->setText(M);
    ui->s->setText(S);
    ui->phi->setText(PHI);
    xPOS=-beta*180/(3.1416);
    yPOS=-n*180/(3.1416);
    zPOS=90-theta*180/3.1416;
    //qDebug()<<"m= "<<m;
    xMoverD();
    yMoverD();
    zMoverD();
    //ui->lcdNumberY->display((double)(xPOS));
}

void MainWindow::on_frontview_clicked()
{
    rtopview    =   false;

}

void MainWindow::on_topview_clicked()
{
    rtopview    =   true;

}
