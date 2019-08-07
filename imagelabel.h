/**
    MissionPlanner
    imagelabel.h
    Purpose: ImageLabel class declaration

    @author Edin Guso
    @version 1.0
    @date 17/07/2019
*/


#ifndef IMAGELABEL_H
#define IMAGELABEL_H


#include <QLabel>
#include <QMouseEvent>
#include "supportstructs.h"


/**
    @class ImageLabel
    @inherits QLabel
    Used to display a map and to detect all mouse events
    done on the map such as scrolling in/out and clicking

    @variable<display> stores information about widget position and size
*/
class ImageLabel : public QLabel {
    Q_OBJECT
public:
    /**
        Constructor that can be called with no parameters as well as more parameters. It initializes
        all the variables

        @param<display> initializes @variable<display>
        @param<image> is set as the background image
        @param<parent> passed onto to the @class<QLabel> constructor
    */
    ImageLabel(DisplayInfo display = DisplayInfo(), QImage image = QImage(), QWidget *parent = nullptr);


    /**
        Moves and resizes the checkbox. This function is also called form the construction for the
        initial setup.
    */
    void updateLabel();


    /**
        Changes the display variable but nothing changes visually unless the update functions is called.

        @param<display> is the new value for @variable<display>
    */
    void setDisplay(DisplayInfo display);


    /**
        @return @variable<display>
    */
    DisplayInfo getDisplay() const;

signals:
    /**
        Signal that mousewheel has been scrolled forward

        @param<mousePos> stores the local position of the mouse
    */
    void zoomIn(QPoint mousePos);


    /**
        Signal that mousewheel has been scrolled backward

        @param<mousePos> stores the local position of the mouse
    */
    void zoomOut(QPoint mousePos);


    /**
        Signal that mouse left button has been pressed

        @param<mousePos> stores the local position of the mouse
    */
    void mouseLeftClick(QPoint mousePos);


    /**
        Signal that mouse right button has been pressed

        @param<mousePos> stores the local position of the mouse
    */
    void mouseRightClick(QPoint mousePos);


private:
    /**
        When a QWheelEvent is recieved in the system, this function filters it and sends proper signals

        @param<event> is the event the system recieved
    */
    void wheelEvent(QWheelEvent *event);


    /**
        When a QMouseEvent is recieved in the system, this function filters it and sends proper signals

        @param<event> is the event the system recieved
    */
    void mousePressEvent(QMouseEvent *event);


    DisplayInfo display;
};


#endif // IMAGELABEL_H
