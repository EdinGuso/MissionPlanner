/**
    MissionPlanner
    reachpoints.h
    Purpose: ReachPoints class declaration

    @author Edin Guso
    @version 1.0
    @date 29/07/2019
*/


#ifndef REACHPOINTS_H
#define REACHPOINTS_H


#include "shape.h"


/**
    @enum reachMissionType
    Used for storing the reach mission's type and displaying it accordingly
*/
enum reachMissionType { InOrderGoOnce, NoOrderGoOnce, InOrderGoAlways, NoOrderGoAlways };


/**
    @class ReachPoints
    @inherits Shape
    handles all the reach points geometric information storage, drawing and click detection

    @variable<points> every visit point of the mission in pixel values (pixel information on the original image size)
    @variable<reachType> stores which type of reach mission type the mission is
*/
class ReachPoints : public Shape
{
public:
    /**
        Constructor. It initializes all the variables

        @param<points> initializes @variable<points>
        @param<type> passed onto to the @class<Shape> constructor
        @param<reachType> initializes @variable<reachType>
        @param<borderColor> paseed onto the @class<Shape> constructor
        @param<fillColor> paseed onto the @class<Shape> constructor
    */
    ReachPoints(QVector<QPointF> points, missionType type, reachMissionType reachType,
                QColor borderColor = QColor(), QColor fillColor = QColor());


    /**
        Checks whether the given point is inside the reach points or not

        @param<point> point that needs to be checked
        @param<zoomAmount> the amount that the map has been zoomed
        @return true if the point is inside the reach points
    */
    bool isInside(QPointF point, double zoomAmount);


    /**
        Draws the reach points onto the given image

        @param<img> image pointer
    */
    void drawShape(QImage *img);


    /**
        Draws the reach points using the given paint device (paint device has a pointer to the image)

        @param<painter> paint device pointer
    */
    void drawShape(QPainter *painter);


    /**
        Draws highlighted reach points onto the given image

        @param<img> image pointer
        @param<thickness> thickness of the pen that will be highlighting (1 is the regular thickness)
    */
    void highlight(QImage *img, int thickness);


    /**
        Draws highlighted reach points using the given paint device (paint device has a pointer to the image)

        @param<painter> paint device pointer
        @param<thickness> thickness of the pen that will be highlighting (1 is the regular thickness)
    */
    void highlight(QPainter *painter, int thickness);


    /**
        Moves the reach points by point amount. However, this is not a visual change only numerical
        The image can be updated by calling one of the drawShape methods

        @param<point> the amount the reach points will move
    */
    void move(QPoint point);


    /**
        Used when saving the project to a file. Prints the value about the reach points to the given stream

        @param<stream> text stream to which the information will be written
    */
    void printParameters(QTextStream &stream);


private:
    /**
        If it is a go once mission, this function is called from drawShape

        @param<painter> paint device pointer
        @param<point> the point where the X will be drawn
    */
    void drawX(QPainter *painter, QPointF point);


    /**
        If it is a go always mission, this function is called from drawShape

        @param<painter> paint device pointer
        @param<point> the point where the double circle will be drawn
    */
    void drawDoubleCircle(QPainter *painter, QPointF point);


    QVector<QPointF> points;
    reachMissionType reachType;
};


#endif // REACHPOINTS_H
