#include "headrectitem.h"

headRectItem::headRectItem(qreal x, qreal y, qreal width, qreal height,QBrush brush,QPen pen,
                           QGraphicsItem *parent )
    : QGraphicsRectItem(x,y,width,height,parent)
{
    setToolTip("用来显示卫星数据的地方");
    setCursor(Qt::OpenHandCursor);
    xx=x;
    yy=y;
    w=width;
    h=height;
    b=brush;
    p=pen;
}

QRectF headRectItem::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(0 - penWidth / 2, 0 -penWidth / 2,
                  20 + penWidth, 20 + penWidth);
}

void headRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
//    Q_UNUSED(option);
//    Q_UNUSED(widget);
    painter->setPen(p);
    painter->setBrush(b);
    painter->drawRect(xx,yy,20,20);
}
