#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "QDebug"
#include "QDateTime"

#include "rectfiller.hpp"
#include "sortthread.hpp"
#include "unit.h"

constexpr size_t PADDING = 10;

namespace Settings
{
    //TODO
    // array_size, sleep_time, padding
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
    int window_h = 0,
        window_w = 0;
    const int array_size = 60;

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
