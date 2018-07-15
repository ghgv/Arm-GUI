#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void parse(char *cmd);
    int serialreceived();
    int received();
    void serialM17();
    void setzero();

    void serialM18();
    void xMover(int);
    void yMover(int);
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
    void Mover();
    void mover(std::string& );
    void actuator(float ,float ,float );
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
