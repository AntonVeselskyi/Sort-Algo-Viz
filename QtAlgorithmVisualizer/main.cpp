#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene mainScene;
    MainWindow main_window(&mainScene);
    return a.exec();
}
