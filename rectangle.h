/**
    MissionPlanner
    rectangle.h
    Purpose: Rectangle class declaration

    @author Edin Guso
    @version 1.0
    @date 18/07/2019
*/


#ifndef RECTANGLE_H
#define RECTANGLE_H


#include "shape.h"


/**
    @class Rectangle
    @inherits Shape
    handles all the rectangle geometric information storage, drawing and click detection

    @variable<topLeft> top left corner position of the rectangle (pixel information on the original image size)
    @variable<bottomRight> bottom right corner position of the rectangle (pixel information on the original image size)
*/
class Rectangle : public Shape
{
public:
    /**
        Constructor. It initializes all the variables

        @param<point1> is used in calculating @variable<topLeft> and @variable<bottomRight>
        @param<point2> is used in calculating @variable<topLeft> and @variable<bottomRight>
        @param<type> passed onto to the @class<Shape> constructor
        @param<borderColor> paseed onto the @class<Shape> constructor
        @param<fillColor> paseed onto the @class<Shape> constructor
    */
    Rectangle(QPointF point1, QPointF point2, missionType type,
              QColor borderColor = QColor(), QColor fillColor = QColor());


    /**
        Checks whether the given point is inside the rectangle or not

        @param<point> point that needs to be checked
        @param<zoomAmount> the amount that the map has been zoomed
        @return true if the point is inside the rectangle
    */
    bool isInside(QPointF point, double zoomAmount);


    /**
        Draws the rectangle onto the given image

        @param<img> image pointer
    */
    void drawShape(QImage *img);


    /**
        Draws the rectangle using the given paint device (paint device has a pointer to the image)

        @param<painter> paint device pointer
    */
    void drawShape(QPainter *painter);


    /**
        Draws a highlighted rectangle onto the given image

        @param<img> image pointer
        @param<thickness> thickness of the pen that will be highlighting (1 is the regular thickness)
    */
    void highlight(QImage *img, int thickness);


    /**
        Draws a highlighted rectangle using the given paint device (paint device has a pointer to the image)

        @param<painter> paint device pointer
        @param<thickness> thickness of the pen that will be highlighting (1 is the regular thickness)
    */
    void highlight(QPainter *painter, int thickness);


    /**
        Moves the rectangle by point amount. However, this is not a visual change only numerical
        The image can be updated by calling one of the drawShape methods

        @param<point> the amount the rectangle will move
    */
    void move(QPoint point);


    /**
        Used when saving the project to a file. Prints the value about the rectangle to the given stream

        @param<stream> text stream to which the information will be written
    */
    void printParameters(QTextStream &stream);


private:
    QPointF topLeft;
    QPointF bottomRight;
};


#endif // RECTANGLE_H
