/**
    MissionPlanner
    polygon.cpp
    Purpose: Polygon class implementation

    @author Edin Guso
    @version 1.0
    @date 18/07/2019
*/


#include "polygon.h"


Polygon::Polygon(QVector<QPointF> points, missionType type, QColor borderColor, QColor fillColor)
    : Shape(type, borderColor, fillColor), points(points) {}


bool Polygon::isInside(QPointF point, double zoomAmount) {
    ///calls the helper function from polygondetection.h
    if(isInsideHelper(points, point / zoomAmount))
        return true;
    return false;
}


void Polygon::drawShape(QImage *img) {
    QPainter painter(img);
    painter.setPen(getBorderColor());
    QBrush brush(getFillColor(), getBrushStyle());
    painter.drawPolygon(points);
    QPainterPath path;
    path.addPolygon(points);
    painter.fillPath(path, brush);
}


void Polygon::drawShape(QPainter *painter) {
    painter->setPen(getBorderColor());
    QBrush brush(getFillColor(), getBrushStyle());
    painter->drawPolygon(points);
    QPainterPath path;
    path.addPolygon(points);
    painter->fillPath(path, brush);
}


void Polygon::highlight(QImage *img, int thickness) {
    QPainter painter(img);
    QPen pen(getBorderColor());
    pen.setWidth(thickness);
    painter.setPen(pen);
    painter.drawPolygon(points);
}


void Polygon::highlight(QPainter *painter, int thickness) {
    QPen pen(getBorderColor());
    pen.setWidth(thickness);
    painter->setPen(pen);
    painter->drawPolygon(points);
}


void Polygon::move(QPoint point) {
    for(int i = 0; i < points.size(); i++) {
        ///rx and ry methods return a reference to the values, therefore it is possible to use += operator
        points[i].rx() += point.x();
        points[i].ry() += point.y();
    }
}


void Polygon::printParameters(QTextStream &stream) {
    stream << "Polygon " << getMissionType() << " ";
    for (int i = 0; i <points.size(); i++)
        stream << points[i].x() << " " << points[i].y() << " ";
}
