#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    uint8_t argc, i = 0;
    char *argv[30];
    static void execute();
    PyObject* uarm_foo(PyObject* self, PyObject* args);
    static  PyObject* uarm_gears(PyObject* self, PyObject* args);
    static  PyObject* Kalculate(PyObject* self, PyObject* args);
    void calcular(float x, float y,float z);
    void moving(float x, float y , float z);

private Q_SLOTS:
    void parse(char *cmd);
    int serialreceived();
    int received();
    void serialM17();
    void setzero();

    void serialM18();
    void xMover(int);
    void yMover(int);
    void zMover(int);
    void Girar_wrist(int);
    void Abrir_grip(int);
    void yMoverU();
    void yMoverD();
    void xMoverU();
    void xMoverD();
    void zMoverU();
    void zMoverD();
    void updateg();
    void updateg2();
    void feedbacks();
    void calculate();
    void RMCW(); //Right motor clockwise
    void RMACW(); //Right motor anticlockwise
    void LMCW(); //L motor
    void LMACW(); //L motor
    void ZMACW();//Z Motor acw
    void ZMCW();//Z Motor acw

    void Arriba();
    void Abajo();
    void Mover();
    void mover(std::string& );
    void actuator(float ,float ,float );
    void konnect();
    void python();
    void on_UARM_clicked();
    void on_DARM_clicked();
    void on_ORTHO_clicked();
    void on_PROJECTED_clicked();

    void on_frontview_clicked();

    void on_topview_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
