#include <stdio.h>
#include <QDialog>
#include <QMessageBox>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <fstream>      // std::ifstream
#include <iostream>
#include <fstream>
#include <QOpenGLWidget>
#include "bad.h"


extern QString pathStr;
extern int solidos;


using namespace std;

Bad::Bad(QObject *parent) : QObject(parent)
{
  //head=(Bad::header *)malloc(sizeof(header));
}

Bad::Bad(float angle,float height){
 this->Angle=angle;
 this->Height=height;
}

int Bad::loader(char *name,unsigned long color,float offsetX, float offsetY,float offsetZ)
{

    QMessageBox msgBox;
    char * memblock;
    char * memblock2;
    int j;

    R=(color>>16);
    G=(color>>8) & (0x0000FF);
    B=(color&0x0000FF);

    offX=offsetX;
    offY=offsetY;
    offZ=offsetZ-Height;

    deltaX=0;
    deltaY=0;
    deltaZ=0;

    deltaX1=0;
    deltaY1=0;
    deltaZ1=0;

    degree=Angle;
    degreeZ=0;

    QString str=name;
    QFile Source_file;
    QString fileOut = "C:/Users/German/Documents/programacion/build-test-Desktop_Qt_5_6_2_MinGW_32bit-Debug/debug/" + str;

    Source_file.setFileName(fileOut);
    if (Source_file.open(QIODevice::ReadOnly)) {
                qint32 intb;
                unsigned long a;
                QByteArray bar = Source_file.read(Source_file.size());
                 qint32 x;
                memblock2 = new char [Source_file.size()];

                 memcpy(&head, bar.constData(), sizeof(header));
                 memcpy(memblock2,bar.constData(),Source_file.size());
                   qDebug()<<"filezies "<<Source_file.size();
                 for(int i=0;i<sizeof(header);i++)
                  cout << head.header[i];
                     a=head.number_of_triangles;
                  cout << "Num triangles " << a <<endl;
                  solid=(triangle *)malloc(head.number_of_triangles*sizeof(triangle));
                  memcpy(solid, memblock2+sizeof(header), head.number_of_triangles*sizeof(triangle));
                  idnumber=solidos++;

            }
    triangle *old_solid;
    old_solid=solid;
   /* for(j=0;j<head.number_of_triangles;j++)
        {

        printf("Normal %f %f %f\n",solid->Normal[0],solid->Normal[1],solid->Normal[2]);
        printf("V1 %f %f %f\n",solid->Vertex1[0],solid->Vertex1[1],solid->Vertex1[2]);
        printf("V2 %f %f %f\n",solid->Vertex2[0],solid->Vertex2[1],solid->Vertex2[2]);
        printf("V3 %f %f %f\n",solid->Vertex3[0],solid->Vertex3[1],solid->Vertex3[2]);
        solid++;
        }*/
    solid=old_solid;

            Source_file.close();


return 0;
}

void Bad::draw()
{
    glLoadName(idnumber);
   // qDebug()<<"Solid id: "<<idnumber<<"\n";
    triangle *old_solid;
    old_solid=solid;

    glColor3f(R/255, G/255, B/255);
    GLfloat qaBlack[]= {0,0,0,1};
    GLfloat qaGreen[]= {0,1,0,1};
    GLfloat qaWhite[]= {1,1,1,1};

    glMaterialfv(GL_FRONT,GL_AMBIENT,qaGreen);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,qaGreen);
    glMaterialfv(GL_FRONT,GL_SPECULAR,qaWhite);
    glMaterialf(GL_FRONT,GL_SHININESS,60.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
            glRotatef(degreeZ,0,0,1);
            glTranslatef(deltaX+deltaX1, deltaY+deltaY1, deltaZ+deltaZ1);

            glTranslatef(offX, offY, offZ);
            glRotatef(degree,1,0,0);
            glTranslatef(-offX, -offY, -offZ); //move object to center



    glBegin(GL_TRIANGLES);
    for(int j=0;j<head.number_of_triangles;j++)
        {
        glNormal3f(solid->Normal[0],solid->Normal[1],solid->Normal[2]);
        glVertex3f(solid->Vertex1[0]+posX,solid->Vertex1[1]+posY,solid->Vertex1[2]);
        glVertex3f(solid->Vertex2[0]+posX,solid->Vertex2[1]+posY,solid->Vertex2[2]);
        glVertex3f(solid->Vertex3[0]+posX,solid->Vertex3[1]+posY,solid->Vertex3[2]);
        solid++;
        }

        glEnd();
        glPopMatrix();
        solid=old_solid;

}
