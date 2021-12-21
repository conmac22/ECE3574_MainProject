//System includes
#include <cmath>

//QT includes
#include <QDebug>
#include <QPainter>

//Module includes
#include "qgraphics_arc_item.hpp"

QGraphicsArcItem::QGraphicsArcItem(qreal x, qreal y, qreal width, qreal height,
                                   QGraphicsItem *parent): QGraphicsEllipseItem(
                                     x, y, width, height, parent)
{
}

void QGraphicsArcItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem* option,
                             QWidget* widget)
{
  painter->setPen(pen());
  painter->setBrush(brush());
  painter->drawArc(rect(), startAngle(), spanAngle());
}
