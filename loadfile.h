#ifndef LOADFILE_H
#define LOADFILE_H

#include <QObject>
#include <fstream>      // std::ifstream
#include <iostream>     // std::cout



void loadFile(const char* fn,std::string& str);

#endif // LOADFILE_H
