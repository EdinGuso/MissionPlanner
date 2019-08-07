/**
    MissionPlanner
    supportstructs.h
    Purpose: DisplayInfo and TextInfo structs declaration & implementation

    @author Edin Guso
    @version 1.0
    @date 16/07/2019
*/


#ifndef SUPPORTSTRUCTS_H
#define SUPPORTSTRUCTS_H


#include <QPoint>
#include <QSize>
#include <QString>
#include <QFont>


/**
    @struct DisplayInfo
    Can hold the information
    needed to display the widgets

    @variable<pos> position of the widget
    @variable<size> size of the widget
*/
struct DisplayInfo
{
    QPoint pos;
    QSize size;
    DisplayInfo(QPoint pos = QPoint(), QSize size = QSize()) : pos(pos), size(size) {}
};


/**
    @struct TextInfo
    Can hold the information
    needed to display text on widgets

    @variable<message> text content
    @variable<font> text font
*/
struct TextInfo
{
    QString message;
    QFont font;
    TextInfo(QString message = QString(), QFont font = QFont()) : message(message), font(font) {}
};


/**
    @enum operation
    Used for storing the operation types. Stored in a stack and used for undo operation
*/
enum operation { VehiclePlaced, ShapePlaced, VehicleRemoved, ShapeRemoved };


/**
    @enum vehicleType
    Used for storing the type of the vehicle
*/
enum vehicleType { typeRover = 1, typeQuad = 2, typeFwuav = 3 };


#endif // SUPPORTSTRUCTS_H
