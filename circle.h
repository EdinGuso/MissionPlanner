/**
    MissionPlanner
    circle.h
    Purpose: Circle class declaration

    @author Edin Guso
    @version 1.0
    @date 18/07/2019
*/


#ifndef CIRCLE_H
#define CIRCLE_H


#include "shape.h"


/**
    @class Circle
    @inherits Shape
    Handles all the circle geometric information storage, drawing and click detection

    @variable<center> center position of the circle (pixel information on the original image size)
    @variable<radius> radius length of the circle (pixel information on the original image size)
*/
class Circle : public Shape
{
public:
    /**
        Constructor. It initializes all the variables

        @param<center> initializes @variable<center>
        @param<radius> initializes @variable<radius>
        @param<type> passed onto to the @class<Shape> constructor
        @param<borderColor> paseed onto the @class<Shape> constructor
        @param<fillColor> paseed onto the @class<Shape> constructor
    */
    Circle(QPointF center, double radius, missionType type,
           QColor borderColor = QColor(), QColor fillColor = QColor());


    /**
        Checks whether the given point is inside the circle or not

        @param<point> point that needs to be checked
        @param<zoomAmount> the amount that the map has been zoomed
        @return true if the point is inside the circle
    */
    bool isInside(QPointF point, double zoomAmount);


    /**
        Draws the circle onto the given image

        @param<img> image pointer
    */
    void drawShape(QImage *img);


    /**
        Draws the circle using the given paint device (paint device has a pointer to the image)

        @param<painter> paint device pointer
    */
    void drawShape(QPainter *painter);


    /**
        Draws a highlighted circle onto the given image pointer

        @param<img> image pointer
        @param<thickness> thickness of the pen that will be highlighting (1 is the regular thickness)
    */
    void highlight(QImage *img, int thickness);


    /**
        Draws a highlighted circle using the given paint device (paint device has a pointer to the image)

        @param<painter> paint device pointer
        @param<thickness> thickness of the pen that will be highlighting (1 is the regular thickness)
    */
    void highlight(QPainter *painter, int thickness);


    /**
        Moves the circle center by point amount. However, this is not a visual change only numerical
        The image can be updated by calling one of the drawShape methods

        @param<point> the amount the circle will move
    */
    void move(QPoint point);


    /**
        Used when saving the project to a file. Prints the value about the circle to the given stream

        @param<stream> text stream to which the information will be written
    */
    void printParameters(QTextStream &stream);


private:
    QPointF center;
    double radius;
};


#endif // CIRCLE_H






