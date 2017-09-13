#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "QDebug"
#include "QDateTime"

#include "rectfiller.hpp"
#include "sortthread.hpp"
#include "unit.h"

#define PADDING 10

struct Settings
{
 // array_size
};

struct SortInfo
{
    SortInfo(QGraphicsTextItem*, QGraphicsTextItem*, QGraphicsTextItem*, QGraphicsTextItem*);
    QGraphicsTextItem* comp_text;
    QGraphicsTextItem* comp_number;
    int comp_count;
    QGraphicsTextItem* assign_text;
    QGraphicsTextItem* assign_number;
    int assign_count;
    void plus_comp();
    void plus_assign();
};

class MainWindow : public QGraphicsView
{
    Q_OBJECT


private:
    int window_h = 0;
    int window_w = 0;
    int array_size = 25;

    //leftbottom column position
    float shift;
    float column_width;
    QDateTime start; //start time

    // the scene to draw
    QGraphicsScene* main_scene;
    // the thread that runs the sorting
    SortThread* sort_thread;

    // the array to be sorted
    Unit* array;

    RectFiller *filler;

    SortInfo* stat;
    void init_stat();

public:

    MainWindow(QGraphicsScene* mainScene);
    ~MainWindow();
    inline void setSizeDependentVars()
    { shift = window_w/array_size; column_width = shift-PADDING; }

public slots:
    // changes the rectangle item's height in the GraphicsScene. 0 < relativeHeight < maximumHeight ( = windowHeight)
    void assign_slot(QGraphicsRectItem* item,  QGraphicsTextItem* text, float relativeHeight);
    void comp_slot(QGraphicsRectItem* item1, QGraphicsRectItem* item2);
    void ending_slot();

};


//sort funcs

//BUBBLE
template<class T> inline
    void bubble_sort(T first, T last)
    {
        bool sort_needed = true;
        int j =0;
        while(sort_needed)
        {
            sort_needed = false;

            for(T i = first  ; i != last -1-j; ++i)
            if(i[1]<i[0])
         {
                sort_needed=true;
                qSwap(i[0],i[1]);
         }

            j++;

        }

    }

//SELECTION SORT
template<class T> inline
    void selection_sort(T first, T last)
{
    T min = first;

    int head_element = 0;
   while (min != last)
    {

        for(T i = min+1 /*next after min*/  ; i != last; ++i)
        {
             if(*i < *min)
            {
               // qDebug
                min = i;

            }

        }

    qSwap(first[head_element], *min);
    head_element++;
    min = (first + head_element);
    }
}

//INSERTION SORT
template<class T> inline
    void insert_sort(T first, T last)
{
    for(T i = first+1; i != last-1; ++i)
    {
        T tmp = i;
        while(i>=first)
        {
            if(i[1]<i[0])
                qSwap(i[1],i[0]);
            i--;
        }
        i=tmp;
    }
}

//SHELL SORT
template<class T> inline
    void shell_sort(T first, T last)
{
    int N = (last-first);
    qDebug() <<"size: " << N;
  for (int d = N/2; d >= 1; d /= 2)
      for (int i = d; i < N; i++)
          for (int j = i; j >= d; j -= d)
              if(first[j-d] > first[j])
                  qSwap(first[j-d], first[j]);

}

#endif // MAINWINDOW_H
