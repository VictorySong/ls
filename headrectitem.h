#ifndef HEADRECTITEM_H
#define HEADRECTITEM_H

#include <QGraphicsItem>;
#include <QGraphicsRectItem>
#include <QPainter>
#include <QRect>
#include <QRectF>
#include <QtGui>


class headRectItem : public QGraphicsRectItem
{
public:
    headRectItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);

protected:

};

#endif // HEADRECTITEM_H
