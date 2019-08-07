/**
    MissionPlanner
    vehicle.cpp
    Purpose: Vehicle class implementation

    @author Edin Guso
    @version 1.0
    @date 18/07/2019
*/


#include "vehicle.h"


Vehicle::Vehicle(DisplayInfo display, QPixmap pix, QColor borderColor, QColor fillColor, int num, vehicleType vehicleT)
    : display(display), borderColor(borderColor), fillColor(fillColor), vehicleNumber(num), vehicleT(vehicleT) {

    if(!pix.isNull())
        this->pix = pix.scaled(display.size.width(),display.size.height(),Qt::KeepAspectRatio);
}


Vehicle::~Vehicle() {}


bool Vehicle::isInside(QPointF point, double zoomAmount) {
    ///both blocks of code do the same thing. i chose division because it has less operations
    //if(point.x() > display.pos.x() * zoomAmount &&
    //        point.y() > display.pos.y() * zoomAmount &&
    //        point.x() < display.pos.x() * zoomAmount + display.size.width() * zoomAmount &&
    //        point.y() < display.pos.y() * zoomAmount + display.size.height() * zoomAmount)
    if(point.x() / zoomAmount > display.pos.x() &&
            point.y() / zoomAmount > display.pos.y() &&
            point.x() / zoomAmount < display.pos.x() + display.size.width() &&
            point.y() / zoomAmount < display.pos.y() + display.size.height())
        return true;
    return false;
}


QVector<Shape*>* Vehicle::getShapes() {
    return &shapes;
}


void Vehicle::draw(QImage *img) {
    QPainter painter(img);
    painter.drawImage(display.pos, (pix.scaled(int(display.size.width()),
                                                            int(display.size.height()),
                                                            Qt::KeepAspectRatio)).toImage());

    painter.setPen(getBorderColor());
    painter.drawRect(QRectF(QPoint(display.pos.x(), display.pos.y()), QPoint(display.pos.x() + display.size.width(), display.pos.y() + display.size.height())));

    for(int i = 0; i < shapes.size(); i++)
        shapes[i]->drawShape(&painter);
}


void Vehicle::drawWithoutVehicle(QImage *img) {
    QPainter painter(img);
    painter.setPen(getBorderColor());
    for(int i = 0; i < shapes.size(); i++)
        shapes[i]->drawShape(&painter);
}


void Vehicle::drawWithoutShape(QImage *img, Shape *shape) {
    QPainter painter(img);
    painter.drawImage(display.pos, (pix.scaled(int(display.size.width()),
                                                            int(display.size.height()),
                                                            Qt::KeepAspectRatio)).toImage());

    painter.setPen(getBorderColor());
    painter.drawRect(QRectF(QPoint(display.pos.x(), display.pos.y()), QPoint(display.pos.x() + display.size.width(), display.pos.y() + display.size.height())));

    for(int i = 0; i < shapes.size(); i++)
        if(shapes[i] != shape)
            shapes[i]->drawShape(&painter);
}


void Vehicle::addShape(Shape *shape) {
    shapes.push_back(shape);
}


QPixmap Vehicle::getPixmap() {
    return pix;
}


void Vehicle::setParameters() {}


void Vehicle::setParameters(QVector<double> vals) {
    parameters = vals;
}


void Vehicle::printParameters(QTextStream &stream) {
    stream << "Parameters ";
    for (int i = 0; i < parameters.size(); i++) {
        stream << parameters[i] << " ";
    }
}


void Vehicle::setDisplayInfo(DisplayInfo display) {
    this->display = display;
}


DisplayInfo Vehicle::getDisplayInfo() {
    return display;
}


void Vehicle::setBorderColor(QColor color) {
    borderColor = color;
}


void Vehicle::setFillColor(QColor color) {
    fillColor = color;
}


QColor Vehicle::getBorderColor() {
    return borderColor;
}


QColor Vehicle::getFillColor() {
    return fillColor;
}


void Vehicle::setvehicleNumber(int num) {
    vehicleNumber = num;
}


int Vehicle::getVehicleNumber() {
    return vehicleNumber;
}


void Vehicle::setVehicleType(vehicleType vehicleT) {
    this->vehicleT = vehicleT;
}


vehicleType Vehicle::getVehicleType() {
    return  vehicleT;
}


void Vehicle::highlight(QImage *img, int thickness) {
    QPainter painter(img);
    QPen pen(getBorderColor());
    pen.setWidth(thickness);
    painter.setPen(pen);
    painter.drawRect(QRectF(QPoint(display.pos.x(), display.pos.y()), QPoint(display.pos.x() + display.size.width(), display.pos.y() + display.size.height())));

    for(int i = 0; i < shapes.size(); i++)
        shapes[i]->highlight(&painter, thickness);
}


void Vehicle::move(QPoint point) {
    display.pos.rx() += point.x();
    display.pos.ry() += point.y();
}
