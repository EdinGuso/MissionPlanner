/**
    MissionPlanner
    rover.cpp
    Purpose: Rover class implementation

    @author Edin Guso
    @version 1.0
    @date 19/07/2019
*/


#include "rover.h"


Rover::Rover(DisplayInfo display, QPixmap pix, QColor borderColor, QColor fillColor, int num,
             vehicleType vehicleT) : Vehicle (display, pix, borderColor, fillColor, num, vehicleT) {}


void Rover::setParameters(){
    ///default configuration can be manually entered here (# of numbers must be equal # of parameters)
    parameters.push_back(70);
    parameters.push_back(12);
}
