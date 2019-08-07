/**
    MissionPlanner
    rectangle.cpp
    Purpose: Rectangle class implementation

    @author Edin Guso
    @version 1.0
    @date 18/07/2019
*/


#include "rectangle.h"


Rectangle::Rectangle(QPointF point1, QPointF point2, missionType type, QColor borderColor,
                     QColor fillColor) : Shape(type, borderColor, fillColor) {
    ///given the two points of the rectangle, compute the top left and bottom right points
    if(point1.x() < point2.x() && point1.y() < point2.y()){
        topLeft = point1;
        bottomRight = point2;
    }
    else if(point1.x() > point2.x() && point1.y() > point2.y()){
        topLeft = point2;
        bottomRight = point1;
    }
    else if(point1.x() < point2.x() && point1.y() > point2.y()){
        topLeft.setX(point1.x());
        topLeft.setY(point2.y());
        bottomRight.setX(point2.x());
        bottomRight.setY(point1.y());
    }
    else if(point1.x() > point2.x() && point1.y() < point2.y()){
        topLeft.setX(point2.x());
        topLeft.setY(point1.y());
        bottomRight.setX(point1.x());
        bottomRight.setY(point2.y());
    }
}


bool Rectangle::isInside(QPointF point, double zoomAmount){
    ///if the given point's x and y coordinates are both between top left and bottom right corners'
    ///x and y coordinates, the point is inside
    if(point.x() > topLeft.x() * zoomAmount && point.y() > topLeft.y() * zoomAmount &&
            point.x() < bottomRight.x() * zoomAmount && point.y() < bottomRight.y() * zoomAmount)
        return true;
    return false;
}


void Rectangle::drawShape(QImage *img){
    QPainter painter(img);
    painter.setPen(getBorderColor());
    QBrush brush(getFillColor(), getBrushStyle());
    painter.drawRect(QRectF(topLeft, bottomRight));
    painter.fillRect(QRectF(topLeft, bottomRight), brush);
}


void Rectangle::drawShape(QPainter *painter){
    painter->setPen(getBorderColor());
    QBrush brush(getFillColor(), getBrushStyle());
    painter->drawRect(QRectF(topLeft, bottomRight));
    painter->fillRect(QRectF(topLeft, bottomRight), brush);
}


void Rectangle::highlight(QImage *img, int thickness) {
    QPainter painter(img);
    QPen pen(getBorderColor());
    pen.setWidth(thickness);
    painter.setPen(pen);
    painter.drawRect(QRectF(topLeft, bottomRight));
}


void Rectangle::highlight(QPainter *painter, int thickness) {
    QPen pen(getBorderColor());
    pen.setWidth(thickness);
    painter->setPen(pen);
    painter->drawRect(QRectF(topLeft, bottomRight));
}


void Rectangle::move(QPoint point) {
    ///rx and ry methods return a reference to the values, therefore it is possible to use += operator
    topLeft.rx() += point.x();
    topLeft.ry() += point.y();
    bottomRight.rx() += point.x();
    bottomRight.ry() += point.y();
}


void Rectangle::printParameters(QTextStream &stream) {
    stream << "Rectangle " << getMissionType() << " " << topLeft.x() << " " << topLeft.y() << " " << bottomRight.x() << " " << bottomRight.y();
}
