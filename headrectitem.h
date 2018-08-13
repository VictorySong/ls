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
    headRectItem(qreal x, qreal y, qreal width, qreal height,
                 QBrush brush, QPen pen,QGraphicsItem *parent = nullptr);
    qreal xx;
    qreal yy;
    qreal w;
    qreal h;

//    headRectItem(qreal x, qreal y, qreal width, qreal height,QBrush b,QPen p, QGraphicsItem *parent);
protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QBrush b;
    QPen p;
};

#endif // HEADRECTITEM_H
