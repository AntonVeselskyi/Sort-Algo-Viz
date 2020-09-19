#pragma once

#include <QGraphicsRectItem>
#include <QList>
#include "QBrush"

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
