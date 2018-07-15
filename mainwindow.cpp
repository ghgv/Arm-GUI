#include <QApplication>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QTimer>
#include <string.h>

#include "bad.h"
#include "loadfile.h"


QSerialPort *serial;

QString str;
float yPOS=0;
float xPOS=0;
float zPOS=0;
float a1=0,b1=12,c1=12;
#define ratio 4.0909
bool feedbackb=false;
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


extern int posq;
extern int pos2;
extern float degreeZ;
std::string source;

bool si = false;
bool fin = false;
FILE *outfile;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    loadFile("position.pos",source);



    serial= new QSerialPort(this);
    ui->lcdNumberX->setPalette(Qt::blue);
    ui->lcdNumberY->setPalette(Qt::blue);
    str=ui->SPort->text();
    ui->espera->setText("150");
    serial->setPortName(str);
    serial->setBaudRate(QSerialPort::Baud57600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);
    //serial->write("ok*");
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialreceived()));
    connect(ui->Enabled_B,SIGNAL(pressed()),this,SLOT(serialM17()));
    connect(ui->Disabled_B,SIGNAL(pressed()),this,SLOT(serialM18()));
    connect(ui->SetZero,SIGNAL(pressed()),this,SLOT(setzero()));
    connect(ui->xmover, SIGNAL(valueChanged(int)),this, SLOT(xMover(int)));
    connect(ui->ymover, SIGNAL(valueChanged(int)),this, SLOT(yMover(int)));

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

    //Girar_wrist(0);

    QTimer *timer = new QTimer(this);
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
    sphere->loader("sphere.stl",RED,0,0,0);
    secondtriangle->loader("secondtriangle.stl",GREEN,14.91295,143.89566,93.21524);//done

    SG90->loader("SG90.stl",BLUE,25.16291,186.04488,72.00875);//done
    SG90holder->loader("SG90holder.stl",YELLOW,19.18367,211.53325,93.99998);//done

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
    serial->write("M17\r");
}

void MainWindow::serialM18()
{
    serial->write("M18\r");
}

void MainWindow::setzero()
{
    serial->write("reset\r");
}


void MainWindow::xMover(int x)
{
    QString s= QString::number((float)x/1);
    posq=(float) x;
    s="pos X "+s+"\r";
   // serial->write(s.toStdString().c_str());
    ui->lcdNumberX->display((double)x/10);
}

void MainWindow::yMover(int y) // Moves away
{
   // yPOS=y;
   // QString s= QString::number((float)yPOS/1);
    pos2=(float)y;
    printf("%f\n",pos2);
   // s="pos Y "+s+"\r";
   // serial->write(s.toStdString().c_str());
    serial->flush();
    ui->lcdNumberY->display((double)(y));

}

void MainWindow::yMoverU() //UP
{
    yPOS--;
    float k;
    QString s= QString::number((float)yPOS);
    std::cout << "In yMoverU "<<std::endl;
    s="pos Y "+s+"\r";
    //posq=yPOS*2;
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


    serial->write(s.toStdString().c_str());
    serial->flush();

    ui->plainTextEdit->insertPlainText(s);
    ui->lcdNumberY->display((double)(yPOS/10));
}

void MainWindow::yMoverD() //DOWN
{
    yPOS++;
    QString s= QString::number((float)yPOS);
    std::cout << "In xMOverD "<<std::endl;

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

    serial->write(s.toStdString().c_str());
    serial->waitForBytesWritten(-1);
    serial->flush();

    ui->plainTextEdit->insertPlainText(s);
    ui->lcdNumberY->display((double)(yPOS));
}

void MainWindow::xMoverU()  //FRONT
{
    xPOS=xPOS-1;
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

    serial->write(s.toStdString().c_str());
    serial->waitForBytesWritten(-1);
    serial->flush();

    ui->plainTextEdit->insertPlainText(s);
    ui->lcdNumberY->display((double)(xPOS/10));
}

void MainWindow::xMoverD() //BACK
{
    xPOS=xPOS+1;
    QString s= QString::number((float)xPOS/1);
    std::cout << "In xMOverD "<<std::endl;
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

    serial->write(s.toStdString().c_str());
    serial->waitForBytesWritten(-1);
    ui->plainTextEdit->insertPlainText(s);
    ui->lcdNumberY->display((double)(xPOS/10));
}

void MainWindow::zMoverU()
{
    zPOS=zPOS-1;
    degreeZ=zPOS;
    QString s= QString::number((float)zPOS/1);
    s="pos Z "+s+"\r";
    serial->write(s.toStdString().c_str());
    ui->lcdNumberY->display((double)(zPOS/10));

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
}

void MainWindow::zMoverD() //Moves the object around Z
{
    zPOS=zPOS+1;
    degreeZ=zPOS;
    QString s= QString::number((float)zPOS);
    s="pos Z "+s+"\r";
    serial->write(s.toStdString().c_str());
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
 }

void MainWindow::updateg()
{

    QString s;
    s="backl\r";
    if(feedbackb==true)
    serial->write(s.toStdString().c_str());
    //ui->lcdNumberY->display((double)(zPOS/10));


}


void MainWindow::updateg2()
{

    QString s;
    s="backr\r";
    serial->write(s.toStdString().c_str());
    //ui->lcdNumberY->display((double)(zPOS/10));

}


void MainWindow::feedbacks()
{
    feedbackb=!feedbackb;
}

void MainWindow::calculate()
{
   float x,y,y1,z,alpha,beta,gamma,m,l,c,n,phi,s,theta;
   QString str;
   l=12;
   a1=0;b1=12;c1=12;

   str=ui->X->toPlainText();
   x=str.toDouble();


   str=ui->Y->toPlainText();
   y=str.toDouble();
   str=ui->Z->toPlainText();
   z=str.toDouble();

   s=sqrt(x*x+y*y);
   theta=atan(y/x);
   //// Then change y by s
   m=sqrt(s*s+z*z);



   alpha=acos(m/(2*l));

   //QString M = QString::number(alpha*180/3.1416);
   //ui->m->setText(M);
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
   xMoverD();
   yMoverD();
   zMoverD();
   ui->lcdNumberY->display((double)(xPOS));

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
        } while ((i < 30) && (argv[i] != NULL));

}

void MainWindow::Mover()
{
    mover(source);
}

void MainWindow::mover(std::string& str)
{
   float a,b,c;

   outfile=fopen("C:/Users/German/Documents/programacion/test/bin/Release/out.txt","w");
   if(outfile==NULL)
   {
       printf("No out text");

   }


 fprintf(outfile,"Starting point: X= %0.2f, Y= %0.2f, Z= %0.2f\n",a1,b1,c1);
   float DeltaA,DeltaB,DeltaC;
   float deltaA,deltaB,deltaC;
   float maxvel=0.1;
   float iterations;
   int e;
   char s;
   QString sleep;
    std::ifstream in("C:/Users/German/Documents/programacion/test/bin/Release/position.pos");
    if(!in.is_open())
    {
        std::cout << "The file cannot be opened\n";
        exit(0);
        return;
    }
    char tmp[300];
    do
    {
       in.getline(tmp,300);
       sscanf(tmp,"%s %f %f %f",&s,&a,&b,&c);
       std::cout << s <<" " << a<<" "<< b << " "<<c <<std::endl;
       fprintf(outfile,"\nCommand: %c, X= %f, Y= %f, Z= %f\n",s,a,b,c);
       fprintf(outfile,"a1= %f, b1= %f, c1= %f\n",a1,b1,c1);

       sleep=ui->espera->toPlainText();
        std::cout << ".." <<std::endl;
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
               serial->write(s.toStdString().c_str());
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
    serial->write(s.toStdString().c_str());


}

void MainWindow::Girar_wrist(int a)
{
    QString s= QString::number((int)a);
    s="pos W "+s+"\r";
    serial->write(s.toStdString().c_str());
    ui->plainTextEdit->insertPlainText(s);


}
