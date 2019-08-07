/**
    MissionPlanner
    quad.cpp
    Purpose: Quad class implementation

    @author Edin Guso
    @version 1.0
    @date 19/07/2019
*/


#include "quad.h"


Quad::Quad(DisplayInfo display, QPixmap pix, QColor borderColor, QColor fillColor, int num,
           vehicleType vehicleT) : Vehicle (display, pix, borderColor, fillColor, num, vehicleT) {}


void Quad::setParameters(){
    ///default configuration can be manually entered here (# of numbers must be equal # of parameters)
    parameters.push_back(4);
    parameters.push_back(32);
}
