/**
    MissionPlanner
    reachpoints.cpp
    Purpose: ReachPoints class implementation

    @author Edin Guso
    @version 1.0
    @date 29/07/2019
*/


#include "reachpoints.h"


ReachPoints::ReachPoints(QVector<QPointF> points, missionType type, reachMissionType reachType,
                         QColor borderColor, QColor fillColor)
    : Shape(type, borderColor, fillColor), points(points), reachType(reachType) {}


bool ReachPoints::isInside(QPointF point, double zoomAmount){
    ///if the point is within the small distance of any of the reach points, it is inside
    for(int i = 0; i < points.size(); i++) {
        if(QLineF(points[i], point / zoomAmount).length() < 10)
            return true;
    }
    return false;
}


void ReachPoints::drawShape(QImage *img) {
    const int PEN_WIDTH = 2;

    QPainter painter(img);
    QPen pen(getBorderColor());
    pen.setWidth(PEN_WIDTH);
    painter.setPen(pen);
    if(reachType == InOrderGoOnce || reachType == NoOrderGoOnce)
        for(int i = 0; i < points.size(); i++)
            drawX(&painter, points[i]);
    else
        for(int i = 0; i < points.size(); i++)
            drawDoubleCircle(&painter, points[i]);
    if(reachType == InOrderGoOnce || reachType == InOrderGoAlways)
        for(int i = 0; i + 1 < points.size(); i++)
            painter.drawLine(points[i], points[i + 1]);
}


void ReachPoints::drawShape(QPainter *painter) {
    const int PEN_WIDTH = 2;

    QPen pen(getBorderColor());
    pen.setWidth(PEN_WIDTH);
    painter->setPen(pen);
    if(reachType == InOrderGoOnce || reachType == NoOrderGoOnce)
        for(int i = 0; i < points.size(); i++)
            drawX(painter, points[i]);
    else
        for(int i = 0; i < points.size(); i++)
            drawDoubleCircle(painter, points[i]);
    if(reachType == InOrderGoOnce || reachType == InOrderGoAlways)
        for(int i = 0; i + 1 < points.size(); i++)
            painter->drawLine(points[i], points[i + 1]);
}


void ReachPoints::highlight(QImage *img, int thickness) {
    QPainter painter(img);
    QPen pen(getBorderColor());
    pen.setWidth(thickness);
    painter.setPen(pen);
    if(reachType == InOrderGoOnce || reachType == NoOrderGoOnce)
        for(int i = 0; i < points.size(); i++)
            drawX(&painter, points[i]);
    else
        for(int i = 0; i < points.size(); i++)
            drawDoubleCircle(&painter, points[i]);
    if(reachType == InOrderGoOnce || reachType == InOrderGoAlways)
        for(int i = 0; i + 1 < points.size(); i++)
            painter.drawLine(points[i], points[i + 1]);
}


void ReachPoints::highlight(QPainter *painter, int thickness) {
    QPen pen(getBorderColor());
    pen.setWidth(thickness);
    painter->setPen(pen);
    if(reachType == InOrderGoOnce || reachType == NoOrderGoOnce)
        for(int i = 0; i < points.size(); i++)
            drawX(painter, points[i]);
    else
        for(int i = 0; i < points.size(); i++)
            drawDoubleCircle(painter, points[i]);
    if(reachType == InOrderGoOnce || reachType == InOrderGoAlways)
        for(int i = 0; i + 1 < points.size(); i++)
            painter->drawLine(points[i], points[i + 1]);
}


void ReachPoints::move(QPoint point) {
    for(int i = 0; i < points.size(); i++) {
        points[i].setX(points[i].x() + point.x());
        points[i].setY(points[i].y() + point.y());
    }
}


void ReachPoints::printParameters(QTextStream &stream) {
    stream << "Reach " << getMissionType() << " " << reachType << " ";
    for (int i = 0; i <points.size(); i++)
        stream << points[i].x() << " " << points[i].y() << " ";
}


void ReachPoints::drawX(QPainter *painter, QPointF point) {
    painter->drawLine(QPoint(int(point.x() - 5), int(point.y() - 5)), QPoint(int(point.x() + 5), int(point.y() + 5)));
    painter->drawLine(QPoint(int(point.x() + 5), int(point.y() - 5)), QPoint(int(point.x() - 5), int(point.y() + 5)));
}


void ReachPoints::drawDoubleCircle(QPainter *painter, QPointF point) {
    painter->drawEllipse(point, 5, 5);
    painter->drawEllipse(point, 10, 10);
}
