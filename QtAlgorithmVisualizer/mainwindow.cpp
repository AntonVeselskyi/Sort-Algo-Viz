#include "mainwindow.h"
#include "unit.h"
#include "sortthread.hpp"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTime>
#include <QObject>
#include <QDebug>
#include <QGraphicsScene>
#include <QRectF>
#include <algorithm>
#include <functional>
#include <QApplication>
#include "QDesktopWidget"
#include <QDebug>




MainWindow::MainWindow(QGraphicsScene* mainScene) : QGraphicsView{mainScene}
  ,	main_scene{mainScene}
{
     qsrand(QTime::currentTime().msec());

     init_stat();

     array = new Unit[array_size];
     filler = new RectFiller;
     sort_thread = new SortThread(array, array_size, shell_sort<Unit*>);//bubble_sort<Unit*>);
  qDebug()<< "array: " << array << "array_size: "<< array_size;
     QRect rec =
             QApplication::desktop()->screenGeometry();
      window_h = rec.height()-200;
      window_w = rec.width()-200;
      this->setFixedWidth(window_w);
      this->setFixedHeight(window_h);

      setSizeDependentVars();
      this->show();

      //font
      QFont font;
      font.setPixelSize(10);
      font.setBold(false);
      font.setFamily("Calibri");

      for (auto i = 0; i < array_size; ++i) {
          array[i] = ((int) qrand() % 100);  //  0 < value < 100

          // add the graphic Rectangle Items to the Scene
          QGraphicsRectItem* columnRect = main_scene->addRect(i*shift,  window_h- ((array[i]/100.0)*(float)window_h),
                                                  column_width, (array[i]/100.0)*(float)window_h);
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

      QObject::connect(sort_thread, SIGNAL(end_of_run()), this, SLOT(ending_slot()));

      start = QDateTime::currentDateTime();
      sort_thread->start();




}

MainWindow::~MainWindow()
{
    while (sort_thread->isRunning())
        ;
    delete sort_thread;
    delete[] array;
}

void MainWindow::init_stat() {
    stat = new SortInfo{main_scene->addText("Number of comparisons: "), main_scene->addText("0"),
                           main_scene->addText("Number of assignments: "), main_scene->addText("0")};
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

void SortInfo::plus_comp() {
    comp_count++;
    comp_number->setPlainText(QString::number(comp_count));
}

void SortInfo::plus_assign() {
    assign_count++;
    assign_number->setPlainText(QString::number(assign_count));
}



void MainWindow::assign_slot(QGraphicsRectItem* item,  QGraphicsTextItem* text, float relativeHeight)
{
   filler->clearFill();
   float newHeight =relativeHeight * (float)window_h;
   float x = item->rect().x();
   float y = (float)window_h - newHeight;
   float width = column_width;
   item->setRect(x, y, width, newHeight);
   text->setPos(x, y - 30);
   text->setPlainText(QString::number(relativeHeight*100));

   filler->fillItem(item, Qt::green);

   stat->plus_assign();
}

void MainWindow::comp_slot(QGraphicsRectItem* item1, QGraphicsRectItem* item2)
{
   stat->plus_comp();
}

void MainWindow::ending_slot()
{
    QDateTime finish = QDateTime::currentDateTime();
    int secs = finish.secsTo(start);
    main_scene->addText("\n\n\nSorted in " + QString::number(secs) + " seconds");
   filler->clearFill();

}

