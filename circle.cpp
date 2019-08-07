/**
    MissionPlanner
    circle.cpp
    Purpose: Circle class implementation

    @author Edin Guso
    @version 1.0
    @date 18/07/2019
*/


#include "circle.h"


Circle::Circle(QPointF center, double radius, missionType type, QColor borderColor,
               QColor fillColor): Shape(type, borderColor, fillColor), center(center), radius(radius) {}


bool Circle::isInside(QPointF point, double zoomAmount) {
    ///if the distance from the point to the center is less than the radius, the point is inside
    QLineF line(center * zoomAmount, point);
    if (line.length() < radius)
        return true;
    return false;
}


void Circle::drawShape(QImage *img) {
    QPainter painter(img);
    painter.setPen(getBorderColor());
    QBrush brush(getFillColor(), getBrushStyle());
    ///ellipse with same radii is a circle with that radius
    painter.drawEllipse(center, radius, radius);
    QPainterPath path;
    path.addEllipse(center, radius, radius);
    painter.fillPath(path, brush);
}


void Circle::drawShape(QPainter *painter) {
    painter->setPen(getBorderColor());
    QBrush brush(getFillColor(), getBrushStyle());
    painter->drawEllipse(center, radius, radius);
    QPainterPath path;
    path.addEllipse(center, radius, radius);
    painter->fillPath(path, brush);
}


void Circle::highlight(QImage *img, int thickness) {
    QPainter painter(img);
    QPen pen(getBorderColor());
    pen.setWidth(thickness);
    painter.setPen(pen);
    painter.drawEllipse(center, radius, radius);
}


void Circle::highlight(QPainter *painter, int thickness) {
    QPen pen(getBorderColor());
    pen.setWidth(thickness);
    painter->setPen(pen);
    painter->drawEllipse(center, radius, radius);
}


void Circle::move(QPoint point) {
    ///rx and ry methods return a reference to the values, therefore it is possible to use += operator
    center.rx() += point.x();
    center.ry() += point.y();
}


void Circle::printParameters(QTextStream &stream) {
    stream << "Circle " << getMissionType() << " " << center.x() << " " << center.y() << " " << radius;
}
