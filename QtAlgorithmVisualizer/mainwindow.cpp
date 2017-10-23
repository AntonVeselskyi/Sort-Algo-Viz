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


int* var5(int size) //viluchiti modu
{
    int *arr = new int[size];

    for (int i=0; i <size; i++)
     arr[i] = ((int) qrand() % 100);  //  0 < value < 100

     //find modu
     int good_index = 0, max_counter =0, counter =0;
     for (int i=0; i <size; i++)
     {
         counter=1;
         for (int j=0; j <size; j++)
         {
             if (arr[i] == arr[j])
             {
                 counter ++;
             }
         }

         if (counter>max_counter)
         {
             good_index = i;
             max_counter = counter;
         }
     }


     qDebug() << "moda - " << arr[good_index] << " count: " <<max_counter;
     //remove all modu samples, change them to 1000
     int good_value = arr[good_index];
     for(int i=0; i <size; i++)
     {
         if(arr[i] == good_value)
             arr[i] = 100;
     }

     return arr;
}

#include <QtMath>
int* var6(int size) //sin
{
    int *arr = new int[size];

    for (int i=0; i < size; i++)
     arr[i] = abs(qSin(((int) qrand() % 100))*100);  //  0 < value < 100

    return arr;

}

MainWindow::MainWindow(QGraphicsScene* mainScene) : QGraphicsView{mainScene}
  ,	main_scene{mainScene}
{
     qsrand(QTime::currentTime().msec());

     init_stat();

     array = new Unit[array_size];
     filler = new RectFiller;
     sort_thread = new SortThread(array, array_size, count_sort<Unit*>);//bubble_sort<Unit*>);
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

      int *array_dodatkove = var6(array_size);

      for (auto i = 0; i < array_size; ++i) {
          array[i] = array_dodatkove[i];  //  0 < value < 100

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

      delete array_dodatkove;



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

int mpartition(Unit *arr, const int left, const int right)
{
    const int mid = left + (right - left) / 2;
    const int pivot = arr[mid];
    // move the mid point value to the front.
    std::swap(arr[mid],arr[left]);
    int i = left + 1;
    int j = right;
    while (i <= j) {
        while(i <= j && arr[i] <= pivot) {
            i++;
        }

        while(i <= j && arr[j] > pivot) {
            j--;
        }

        if (i < j) {
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i - 1],arr[left]);
    return i - 1;
}

void mquicksort(Unit *arr, const int left, const int right, const int sz){

    if (left >= right) {

        return;
    }


    int part = mpartition(arr, left, right);


    mquicksort(arr, left, part - 1, sz);
    mquicksort(arr, part + 1, right, sz);
}

//QUICK SORT
template<class T> inline
    void mq_sort(T first, T last)
{
    int N = (last-first); //size
    mquicksort((Unit*)first, 0, N-1, N);
}


