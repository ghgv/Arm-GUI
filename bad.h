#ifndef BAD_H
#define BAD_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <stdio.h>



class Bad : public QObject
{
    Q_OBJECT
public:
    explicit Bad(QObject *parent = 0);
    int loader(char *name,unsigned long color,float offsetX, float offsetY,float offsetZ);



    struct  header
    {
       unsigned char header[80];
       unsigned long number_of_triangles;
    } __attribute__((__packed__));


    struct  triangle
    {
      float Normal[3];
      float Vertex1[3];
      float Vertex2[3];
      float Vertex3[3];
      unsigned short Attr;
    } __attribute__((__packed__));

     header head;
     triangle *solid;
     float R,G,B;
     bool select=false;
     int degree;
     int degreeZ;
     float offX, offY, offZ;
     float posX,posY,posZ;
     float deltaX,deltaY,deltaZ;
     float deltaX1,deltaY1,deltaZ1;

     void draw();

signals:

public slots:
};

#endif // BAD_H
