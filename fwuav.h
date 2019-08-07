/**
    MissionPlanner
    fwuav.h
    Purpose: Fwuav class declaration

    @author Edin Guso
    @version 1.0
    @date 19/07/2019
*/


#ifndef FWUAV_H
#define FWUAV_H


#include "vehicle.h"


/**
    @class Fwuav
    @inherits Vehicle
    handles all the fwuav parameter settings
*/
class Fwuav : public Vehicle
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
    Fwuav(DisplayInfo display = DisplayInfo(), QPixmap pix = QPixmap(), QColor borderColor = QColor(), QColor fillColor = QColor(), int num = int(), vehicleType vehicleT = vehicleType());


    /**
        Sets all the parameters to default parameters (have to be hardcoded in @file<fwuav.cpp>)
    */
    void setParameters();
};


#endif // FWUAV_H
