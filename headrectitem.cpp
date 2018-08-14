#include "headrectitem.h"

headRectItem::headRectItem(qreal x, qreal y, qreal width, qreal height,
                           QGraphicsItem *parent )
    : QGraphicsRectItem(x,y,width,height,parent)
{
    setToolTip("用来显示卫星数据的地方");
    setCursor(Qt::OpenHandCursor);

}

