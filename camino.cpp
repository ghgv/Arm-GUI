#include "camino.h"
#include <QString>
#include <stdio.h>
camino::camino(QString a)
{
    this->path=a;
}

void camino::show(){
   printf("path %s",this->path.toStdString().c_str());
}
