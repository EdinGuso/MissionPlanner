/**
    MissionPlanner
    shape.h
    Purpose: Shape class declaration

    @author Edin Guso
    @version 1.0
    @date 18/07/2019
*/


#ifndef SHAPE_H
#define SHAPE_H


#include <QImage>
#include <QPointF>
#include <QPainter>
#include <QTextStream>


/**
    @enum missionType
    Used for storing the current selected mission in main window and change the shape fill patters accordingly
*/
enum missionType {Reach, Patrol, Nofly, NoMission};


/**
    @class Shape
    handles all the shape color information storage and updates

    @variable<type> stores the mission type
    @variable<borderColor> stores the border color information (depends on the vehicle)
    @variable<fillColor> stores the fill color information (depends on the vehicle)
    @variable<brushStyle> stores the fill style (depends on the mission)
*/
class Shape
{
public:
    /**
        Constructor

        @param<type> initializes @variable<type>
        @param<borderColor> initializes @variable<borderColor>
        @param<fillColor> initializes @variable<fillColor>
    */
    Shape(missionType type, QColor borderColor = QColor(), QColor fillColor = QColor());


    /**
        Virtual destructor
    */
    virtual ~Shape();


    /**
        Virtual function: checks whether the point is inside the shape or not
    */
    virtual bool isInside(QPointF point, double zoomAmount) = 0;


    /**
        Virtual function: draws the shape onto the image
    */
    virtual void drawShape(QImage *img) = 0;


    /**
        Virtual function: draws the shape using the paint device
    */
    virtual void drawShape(QPainter *painter) = 0;


    /**
        Virtual function: highlights the shape onto the image
    */
    virtual void highlight(QImage *img, int thickness) = 0;


    /**
        Virtual function: highlights the shape using the paint device
    */
    virtual void highlight(QPainter *painter, int thickness) = 0;


    /**
        Virtual function: moves the shape
    */
    virtual void move(QPoint point) = 0;


    /**
        Virtual function: prints the shape parameters onto the stream
    */
    virtual void printParameters(QTextStream &stream) = 0;


    /**
        Changes the brush style

        @param<brushStyle> is the new value for @variable<brushStyle>
    */
    void setBrushStyle(Qt::BrushStyle brushStyle);


    /**
        Changes the border color

        @param<color> is the new value for @variable<borderColor>
    */
    void setBorderColor(QColor color);


    /**
        Changes the fill color

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
        @return @variable<brushStyle>
    */
    Qt::BrushStyle getBrushStyle();


    /**
        @return the number of the vehicle this shape belongs to based on its color
    */
    int getVehicleNumber();


    /**
        @return @variable<type>
    */
    missionType getMissionType();


private:
    missionType type;
    QColor borderColor;
    QColor fillColor;
    Qt::BrushStyle brushStyle;
};


#endif // SHAPE_H
