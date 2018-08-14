#ifndef HEADRECTITEM_H
#define HEADRECTITEM_H

#include <QGraphicsItem>;
#include <QGraphicsRectItem>
#include <QPainter>
#include <QRect>
#include <QRectF>
#include <QtGui>
#include "tcpsocket.h"


class headRectItem : public QGraphicsRectItem
{
public:
    headRectItem(qreal x, qreal y, qreal width, qreal height,
                 QHashIterator <QString,tcpsocket *> i,QGraphicsItem *parent = nullptr);

protected:

};

#endif // HEADRECTITEM_H
