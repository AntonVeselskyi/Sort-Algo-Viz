#include "mainwindow.h"
#include "unit.h"
#include "sortthread.hpp"
#include "sort_funcs.hpp"

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QObject>
#include <QDebug>
#include <QGraphicsScene>
#include <QRectF>
#include <QApplication>
#include "QDesktopWidget"
#include <QDebug>
#include <algorithm>
#include <functional>


MainWindow::MainWindow(QGraphicsScene* mainScene) :
                       QGraphicsView{mainScene},
                       main_scene{mainScene}
{
    qsrand(QTime::currentTime().msec());

    stat = new SortInfo{main_scene->addText("Number of comparisons: "), main_scene->addText("0"),
                        main_scene->addText("Number of assignments: "), main_scene->addText("0")};

    array = new Unit[array_size];
    qDebug()<< "array: " << array << "array_size: "<< array_size;
    filler = new RectFiller;

    setWindowState(Qt::WindowMaximized);
    QRect rec = QApplication::desktop()->screenGeometry();
    window_h = rec.height() - 100;
    window_w = rec.width() - 50;
    setSizeDependentVars();

    this->show();

    //font
    QFont font;
    font.setPixelSize(10);
    font.setBold(false);
    font.setFamily("Calibri");

    //allocte array of objects to sort with random values (from 0 to 100)
    for (auto i = 0; i < array_size; ++i)
    {
        array[i] = qrand()%100;  //  0 < value < 100

        // add the graphic Rectangle Items to the Scene
        QGraphicsRectItem* columnRect = main_scene->addRect(i*shift,  window_h - ((array[i]/100.0)*(float)window_h),
                                              column_width, (array[i]/100.0)*(float)window_h);
        //add label only if there less then 70 elements (readability)
        if (array_size<=70)
        {
            QGraphicsTextItem* value_label = new QGraphicsTextItem;
            value_label->setPlainText(QString::number(array[i]));
            value_label->setPos(i*shift,  window_h- ((array[i]/100.0)*(float)window_h) - 30);
            mainScene->addItem(value_label);
            qDebug()<< "x: " << i*shift << "y: "<< window_h-((array[i]/100.0)*window_h)<< " "
                  << column_width << " " <<(array[i]/100.0)*window_h<< array[i] ;

            //then saves the pointers in the array objects
            array[i].set_columnV(value_label);
        }

        array[i].set_columnRect(columnRect);

        // whenever operatorEQ signal is emitted, the changeColumnHeight changes the array object's
        // corresponding graphic (rect) item's height
        QObject::connect(&array[i], SIGNAL(operatorEQ(QGraphicsRectItem*,QGraphicsTextItem*,float)),
                         this, SLOT(assign_slot(QGraphicsRectItem*,QGraphicsTextItem*,float)));
        QObject::connect(&array[i], SIGNAL(operatorCOMP(QGraphicsRectItem*,QGraphicsRectItem*)),
                         this, SLOT(comp_slot(QGraphicsRectItem*,QGraphicsRectItem*)));
    }

}

MainWindow::~MainWindow()
{
    while (sort_thread->isRunning())
        ;
    delete sort_thread;
    delete stat;
    delete filler;
    delete[] array;
}

void MainWindow::startSort()
{
    if(sort_thread)
        delete sort_thread;

    sort_thread = new SortThread(array, array_size, shell_sort<Unit*>);//e.g. bubble_sort<Unit*>
    QObject::connect(sort_thread, SIGNAL(end_of_run(qint64)), this, SLOT(ending_slot(qint64)));
    sort_thread->start();
}

SortInfo::SortInfo(QGraphicsTextItem* compText, QGraphicsTextItem* compNum,
                   QGraphicsTextItem* assignText, QGraphicsTextItem* assignNum) :
                   comp_text{compText}, comp_number{compNum}, comp_count{0},
                   assign_text{assignText}, assign_number{assignNum}, assign_count{0}
{
    comp_text->setPos(0,0);
    comp_number->setPos(comp_text->boundingRect().width(), 0);
    assign_text->setPos(0, comp_text->boundingRect().height());
    assign_number->setPos(assign_text->boundingRect().width(), comp_number->boundingRect().height());
}

void SortInfo::plus_comp()
{
    comp_count++;
    comp_number->setPlainText(QString::number(comp_count));
}

void SortInfo::plus_assign()
{
    assign_count++;
    assign_number->setPlainText(QString::number(assign_count));
}

void MainWindow::assign_slot(QGraphicsRectItem* item,  QGraphicsTextItem* text, float relativeHeight)
{
    filler->clearFill();
    float newHeight = relativeHeight * (float)window_h;
    float x = item->rect().x();
    float y = (float)window_h - newHeight;
    float width = column_width;
    item->setRect(x, y, width, newHeight);
    if (array_size<=70)
    {
        text->setPos(x, y - 30);
        text->setPlainText(QString::number(relativeHeight*100));
    }
    filler->fillItem(item, Qt::green);

    stat->plus_assign();
}

void MainWindow::comp_slot(QGraphicsRectItem* item1, QGraphicsRectItem* item2)
{
    stat->plus_comp();
}

void MainWindow::ending_slot(qint64 ms)
{
    main_scene->addText("\n\n\nSorted in " + QString::number(ms) + "ms");
    filler->clearFill();
}
