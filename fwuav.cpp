/**
    MissionPlanner
    fwuav.cpp
    Purpose: Fwuav class implementation

    @author Edin Guso
    @version 1.0
    @date 19/07/2019
*/


#include "fwuav.h"


Fwuav::Fwuav(DisplayInfo display, QPixmap pix, QColor borderColor, QColor fillColor, int num,
             vehicleType vehicleT) : Vehicle (display, pix, borderColor, fillColor, num, vehicleT) {}


void Fwuav::setParameters(){
    ///default configuration can be manually entered here (# of numbers must be equal # of parameters)
    QVector<double> defaultParameters = {120, 70};
    parameters = defaultParameters;
}
