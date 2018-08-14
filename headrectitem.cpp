#include "headrectitem.h"

headRectItem::headRectItem(qreal x, qreal y, qreal width, qreal height,
                           QHashIterator<QString, tcpsocket *> i, QGraphicsItem *parent )
    : QGraphicsRectItem(x,y,width,height,parent)
{
    setToolTip( i.key() );
    setCursor(Qt::OpenHandCursor);
}

