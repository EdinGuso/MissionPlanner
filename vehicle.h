/**
    MissionPlanner
    vehicle.h
    Purpose: Vehicle class declaration

    @author Edin Guso
    @version 1.0
    @date 18/07/2019
*/

#ifndef VEHICLE_H
#define VEHICLE_H


#include <QImage>
#include <QPixmap>
#include <QTextStream>
#include "supportstructs.h"
#include "shape.h"


/**
    @class Vehicle
    handles all the vehicle geometric information storage, image storage, shape storage, drawing and click detection

    @variable<parameters> stores the parameters for the vehicle
    @variable<display> stores the position and size data
    @variable<pix> stores the pixmap for the vehicle
    @variable<borderColor> stores the border color information
    @variable<fillColor> stores the fill color information
    @variable<shapes> stores the shape information
    @variable<vehicleNumber> stores the vehicle number
    @variable<vehicleT> stores the vehicle type
*/
class Vehicle
{
public:
    Vehicle(DisplayInfo display = DisplayInfo(), QPixmap pix = QPixmap(), QColor borderColor = QColor(), QColor fillColor = QColor(), int num = int(), vehicleType vehicleT = vehicleType());
    virtual ~Vehicle();
    bool isInside(QPointF point, double zoomAmount);
    QVector<Shape*>* getShapes();
    void draw(QImage *img);
    void drawWithoutVehicle(QImage *img);
    void drawWithoutShape(QImage *img, Shape *shape);
    void addShape(Shape *shape);
    QPixmap getPixmap();
    virtual void setParameters();
    void setParameters(QVector<double> vals);
    void printParameters(QTextStream &stream);
    void setDisplayInfo(DisplayInfo display);
    DisplayInfo getDisplayInfo();
    void setBorderColor(QColor color);
    void setFillColor(QColor color);
    QColor getBorderColor();
    QColor getFillColor();
    void setvehicleNumber(int num);
    int getVehicleNumber();
    void setVehicleType(vehicleType vehicleT);
    vehicleType getVehicleType();
    void highlight(QImage *img, int thickness);
    void move(QPoint point);


protected:
    QVector<double> parameters;


private:
    DisplayInfo display;
    QPixmap pix;
    QColor borderColor;
    QColor fillColor;
    QVector<Shape*> shapes;
    int vehicleNumber;
    vehicleType vehicleT;
};


#endif // VEHICLE_H
