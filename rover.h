/**
    MissionPlanner
    rover.h
    Purpose: Rover class declaration

    @author Edin Guso
    @version 1.0
    @date 19/07/2019
*/


#ifndef ROVER_H
#define ROVER_H


#include "vehicle.h"


/**
    @class Rover
    @inherits Vehicle
    handles all the rover parameter settings
*/
class Rover : public Vehicle
{
public:
    /**
        Constructor

        @param<display> passed onto to the @class<Vehicle> constructor
        @param<pix> passed onto to the @class<Vehicle> constructor
        @param<borderColor> passed onto to the @class<Vehicle> constructor
        @param<fillColor> passed onto to the @class<Vehicle> constructor
        @param<num> passed onto to the @class<Vehicle> constructor
        @param<vehicleT> passed onto to the @class<Vehicle> constructor
    */
    Rover(DisplayInfo display = DisplayInfo(), QPixmap pix = QPixmap(), QColor borderColor = QColor(), QColor fillColor = QColor(), int num = int(), vehicleType vehicleT = vehicleType());


    /**
        Sets all the parameters to default parameters (have to be hardcoded in @file<quad.cpp>)
    */
    void setParameters();
};


#endif // ROVER_H
