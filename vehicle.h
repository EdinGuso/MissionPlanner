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
    /**
        Constructor that can be called with no parameters as well as
        more parameters. It initializes all the variables

        @param<display> initializes @variable<display>
        @param<pix> initializes @variable<pix>
        @param<borderColor> initializes @variable<borderColor>
        @param<fillColor> initializes @variable<fillColor>
        @param<num> initializes @variable<vehicleNumber>
        @param<vehicleT> initializes @variable<vehicleT>
    */
    Vehicle(DisplayInfo display = DisplayInfo(), QPixmap pix = QPixmap(), QColor borderColor = QColor(),
            QColor fillColor = QColor(), int num = int(), vehicleType vehicleT = vehicleType());


    /**
        Virtual destructor
    */
    virtual ~Vehicle();


    /**
        Checks whether the given point is inside the vehicle or not

        @param<point> point that needs to be checked
        @param<zoomAmount> the amount that the map has been zoomed
        @return true if the point is inside the vehicle
    */
    bool isInside(QPointF point, double zoomAmount);


    /**
        @return @variable<shapes>
    */
    QVector<Shape*>* getShapes();


    /**
        Draws the vehicle and all its shapes onto the given image

        @param<img> image pointer
    */
    void draw(QImage *img);


    /**
        Draws vehicle's all shapes onto the given image

        @param<img> image pointer
    */
    void drawWithoutVehicle(QImage *img);


    /**
        Draws the vehicle and all its shapes apart from the given shape onto the given image

        @param<img> image pointer
        @param<img> shape that will not be drawn
    */
    void drawWithoutShape(QImage *img, Shape *shape);


    /**
        Adds a new shape to the vehicle

        @param<shape> pointer to the shape to be added
    */
    void addShape(Shape *shape);


    /**
        @return @variable<pix>
    */
    QPixmap getPixmap();


    /**
        Virtual function: sets all the parameters of the vehicle to default parameters
    */
    virtual void setParameters();


    /**
        Sets all the parameters of the vehicle to the given parameters

        @param<vals> given parameters
    */
    void setParameters(QVector<double> vals);


    /**
        Used when saving the project to a file. Prints the parameters of the vehicle to the given stream

        @param<stream> text stream to which the information will be written
    */
    void printParameters(QTextStream &stream);


    /**
        Changes the display variable but it does not change visually until updated

        @param<display> is the new value for @variable<display>
    */
    void setDisplayInfo(DisplayInfo display);


    /**
        @return @variable<display>
    */
    DisplayInfo getDisplayInfo();


    /**
        Changes the border color variable but it does not change visually until updated

        @param<coloe> is the new value for @variable<borderColor>
    */
    void setBorderColor(QColor color);


    /**
        Changes the fill color variable but it does not change visually until updated

        @param<color> is the new value for @variable<fillColor>
    */
    void setFillColor(QColor color);


    /**
        @return @variable<borderColor>
    */
    QColor getBorderColor();


    /**
        @return @variable<fillColor>
    */
    QColor getFillColor();


    /**
        Changes the vehicle number variable

        @param<num> is the new value for @variable<vehicleNumber>
    */
    void setvehicleNumber(int num);


    /**
        @return @variable<vehicleNumber>
    */
    int getVehicleNumber();


    /**
        Changes the vehicle type variable

        @param<vehicleT> is the new value for @variable<vehicleT>
    */
    void setVehicleType(vehicleType vehicleT);


    /**
        @return @variable<vehicleT>
    */
    vehicleType getVehicleType();


    /**
        Draws the highlighted vehicle onto the given image

        @param<img> image pointer
        @param<thickness> thickness of the pen that will be highlighting (1 is the regular thickness)
    */
    void highlight(QImage *img, int thickness);


    /**
        Moves the vehicle by point amount. However, this is not a visual change only numerical
        Visual change can be achieved by calling the draw functions

        @param<point> the amount the vehicle will move
    */
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
