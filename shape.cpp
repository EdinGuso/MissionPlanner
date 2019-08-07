/**
    MissionPlanner
    shape.cpp
    Purpose: Shape class implementation

    @author Edin Guso
    @version 1.0
    @date 18/07/2019
*/


#include "shape.h"


Shape::Shape(missionType type, QColor borderColor, QColor fillColor) : type(type),
    borderColor(borderColor), fillColor(fillColor) {
    ///there is no need for reach to have brush style but i left it there just in case
    if (type == Reach)
        brushStyle = Qt::Dense6Pattern;
    else if (type == Patrol)
        brushStyle = Qt::CrossPattern;
    else if (type == Nofly)
        brushStyle = Qt::BDiagPattern;
}


Shape::~Shape() {}


void Shape::setBorderColor(QColor color){
    borderColor = color;
}


void Shape::setFillColor(QColor color){
    fillColor = color;
}


void Shape::setBrushStyle(Qt::BrushStyle brushStyle){
    this->brushStyle = brushStyle;
}


QColor Shape::getBorderColor(){
    return borderColor;
}


QColor Shape::getFillColor(){
    return fillColor;
}


Qt::BrushStyle Shape::getBrushStyle(){
    return brushStyle;
}

int Shape::getVehicleNumber(){
    if(fillColor == Qt::red)
        return 0;
    else if(fillColor == Qt::darkGreen)
        return 1;
    else if(fillColor == Qt::blue)
        return 2;
    else if(fillColor == Qt::magenta)
        return 3;
    else if(fillColor == Qt::yellow)
        return 4;
    ///it should never reach here
    return -1;
}


missionType Shape::getMissionType() {
    return type;
}
