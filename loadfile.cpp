#include "loadfile.h"
#include <stdio.h>
#include <QtSerialPort/QtSerialPort>



void loadFile(const char* fn,std::string& str)
{
    std::ifstream in("C:/Users/German/Documents/programacion/test/bin/Release/position.pos");
    if(!in.is_open())
    {
        std::cout << "The file " << fn << " cannot be opened\n";
        exit(0);
        return;
    }
    char tmp[300];
    while(!in.eof())
    {
        in.getline(tmp,300);
        str+=tmp;
        str+='\n';
    }
}
