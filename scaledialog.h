/**
    MissionPlanner
    scaledialog.h
    Purpose: ScaleDialog class declaration

    @author Edin Guso
    @version 1.0
    @date 26/07/2019
*/


#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H


#include <QDialog>
#include <QPainter>
#include <QInputDialog>
#include <QMessageBox>
#include "imagelabel.h"


/**
    @class ScaleDialog
    @inherits QDialog
    collects all the scale inputs from the user

    @variable<mapDisplay> stores a pointer to the imageLabel dislpaying the map
    @variable<originalImg> stores the original image
    @variable<numClicks> stores the amount of clicks on the map
    @variable<firstClick> stores the position of the first click
    @variable<pixelLength> reference to where the pixel length of the selected section will be stored
    @variable<realLength> reference to where the real length of the selected section will be stored
    @variable<pixelOrigin> reference to where the pixel value of the origin will be stored
    @variable<displaySize> stores the size of the mapDisplay in mapWindow so that pixel values can be
             correctly mapped from here to there
    @variable<screenSize> stores the size of the user's screen so that the image can be correctly
             scaled and displayed
    @variable<ratio> stores the size ratio between this mapDisplay and mapWindow's mapDisplay
*/
class ScaleDialog : public QDialog
{
    Q_OBJECT
public:
    /**
        Constructor

        @param<img> is set as the background image
        @param<pixelLength> reference to where the pixel length of the selected section will be stored
        @param<realLength> reference to where the real length of the selected section will be stored
        @param<pixelOrigin> reference to where the pixel value of the origin will be stored
        @param<displaySize> size of the mapDisplay in mapWindow
        @param<screenSize> size of the user's screen
    */
    ScaleDialog(QImage img, double &pixelLength, double &realLength, QPoint &pixelOrigin,
                QSize displaySize, QSize screenSize);


private slots:
    /**
        Recieves the mouse left click signal from mapDisplay

        @param<mousePos> is the local position of the cursor
    */
    void mouseLeftClick(QPoint mousePos);


    /**
        Recieves the mouse right click signal from mapDisplay

        @param<mousePos> is the local position of the cursor
    */
    void mouseRightClick(QPoint mousePos);


private:
    ImageLabel *mapDisplay;
    QImage originalImg;
    int numClicks;
    QPoint firstClick;
    double &pixelLength;
    double &realLength;
    QPoint &pixelOrigin;
    QSize displaySize;
    QSize screenSize;
    double ratio;
};


#endif // SCALEDIALOG_H
