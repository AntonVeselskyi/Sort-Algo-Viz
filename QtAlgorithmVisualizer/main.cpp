#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene mainScene;
    MainWindow main_window(&mainScene);
    main_window.startSort();

    return a.exec();
}
