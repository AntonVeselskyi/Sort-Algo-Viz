#ifndef RECTFILLER
#define RECTFILLER

#include <QGraphicsRectItem>
#include <QList>
#include "QBrush"

class RectFiller {
public:
    void fillItem(QGraphicsRectItem* item, QColor color) {
    //    QBrush s();
      //  s.setColor(color);
        item->setBrush(color);
        filledList.append(item);
    }

    void clearFill() {
        for (auto& item : filledList)
            item->setBrush(Qt::white);
        filledList.clear();
    }

private:
    QList<QGraphicsRectItem*> filledList;
};

#endif // RECTFILLER

