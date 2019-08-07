/**
    MissionPlanner
    polygon.h
    Purpose: Polygon class declaration

    @author Edin Guso
    @version 1.0
    @date 18/07/2019
*/


#ifndef POLYGON_H
#define POLYGON_H


#include "shape.h"
#include "polygondetection.h"


/**
    @class Polygon
    @inherits Shape
    handles all the polygon geometric information storage, drawing and click detection

    @variable<points> all corner positions of the polygon (pixel information on the original image size)
*/
class Polygon : public Shape
{
public:
    /**
        Constructor. It initializes all the variables

        @param<points> initializes @variable<points>
        @param<type> passed onto to the @class<Shape> constructor
        @param<borderColor> paseed onto the @class<Shape> constructor
        @param<fillColor> paseed onto the @class<Shape> constructor
    */
    Polygon(QVector<QPointF> points, missionType type,
            QColor borderColor = QColor(), QColor fillColor = QColor());


    /**
        Checks whether the given point is inside the polygon or not

        @param<point> point that needs to be checked
        @param<zoomAmount> the amount that the map has been zoomed
        @return true if the point is inside the polygon
    */
    bool isInside(QPointF point, double zoomAmount);


    /**
        Draws the polygon onto the given image

        @param<img> image pointer
    */
    void drawShape(QImage *img);


    /**
        Draws the polygon using the given paint device (paint device has a pointer to the image)

        @param<painter> paint device pointer
    */
    void drawShape(QPainter *painter);


    /**
        Draws a highlighted polygon onto the given image pointer

        @param<img> image pointer
        @param<thickness> thickness of the pen that will be highlighting (1 is the regular thickness)
    */
    void highlight(QImage *img, int thickness);


    /**
        Draws a highlighted polygon using the given paint device (paint device has a pointer to the image)

        @param<painter> paint device pointer
        @param<thickness> thickness of the pen that will be highlighting (1 is the regular thickness)
    */
    void highlight(QPainter *painter, int thickness);


    /**
        Moves the polygon by point amount. However, this is not a visual change only numerical
        The image can be updated by calling one of the drawShape methods

        @param<point> the amount the polygon will move
    */
    void move(QPoint point);


    /**
        Used when saving the project to a file. Prints the value about the polygon to the given stream

        @param<stream> text stream to which the information will be written
    */
    void printParameters(QTextStream &stream);


private:
    QVector<QPointF> points;
};


#endif // POLYGON_H


