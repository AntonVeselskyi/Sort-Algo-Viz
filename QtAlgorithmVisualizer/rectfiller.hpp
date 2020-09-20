#pragma once

#include <QGraphicsRectItem>
#include <QList>
#include "QBrush"

// class that can fill QGraphicsRectItem with color
// and clear all filled elements (will with white)
class RectFiller
{
public:
    void fillItem(QGraphicsRectItem* item, QColor color)
    {
        item->setBrush(color);
        filledList.append(item);
    }

    void clearFill()
    {
        for (auto& item : filledList)
            item->setBrush(Qt::white);
        filledList.clear();
    }

private:
    QList<QGraphicsRectItem*> filledList;
};
