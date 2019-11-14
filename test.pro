#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T19:37:55
#
#-------------------------------------------------

QT       += core gui opengl serialport


CONFIG += no_keywords -fpermissive

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


LIBS += -LC:/Qt/5.6/mingw492_32/lib/libQt4OpenGL.a -lopengl32 -lglu32 -LC:\SDL2\lib\x86 -lSDL2 -LC:\Python34\libs -lpython34

INCLUDEPATH += C:\SDL2\include C:\Python34\include C:\pybind11\include

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    loadfile.cpp \
    bad.cpp \
    joystick.cpp \
    helper.cpp \
    camino.cpp \
    serial.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    loadfile.h \
    bad.h \
    joystick.h \
    helper.h \
    camino.h \
    uarm.h \
    uarmext.h \
    serial.h

FORMS    += mainwindow.ui

DISTFILES += \
    position.pos
