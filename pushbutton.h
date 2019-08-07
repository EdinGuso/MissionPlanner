/**
    MissionPlanner
    pushbutton.h
    Purpose: PushButton class declaration

    @author Edin Guso
    @version 1.0
    @date 16/07/2019
*/


#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H


#include <QPushButton>
#include "supportstructs.h"


/**
    @class PushButton
    @inherits QPushButton
    Easier to setup the button. Instead of having to call several functions after the constructor
    in QPushButton, you can call the constructor itself to setup the button

    @variable<display> stores information about widget position and size
    @variable<text> stores information about text content and font
    @variable<styleSheetUrl> stores the button background image url
*/
class PushButton : public QPushButton
{
public:
    /**
        Constructor that can be called with no parameters as well as more parameters. It initializes
        all the variables

        @param<display> initializes @variable<display>
        @param<text> initializes @variable<text>
        @param<parent> pointer to the parent of the object, passed onto the @class<QPushButton> constructor
        @param<styleSheetUrl> initializes @variable<styleSheetUtl>
    */
    PushButton(DisplayInfo display = DisplayInfo(), TextInfo text = TextInfo(),
               QWidget *parent = nullptr, QString styleSheetUrl = "");


    /**
        Moves, resizes and sets all the text and stylesheet options for the pushbutton. This function is
        also called form the construction for the initial setup.
    */
    void updateButton();


    /**
        Changes the display variable but nothing changes visually unless the update functions is called.

        @param<display> is the new value for @variable<display>
    */
    void setDisplay(DisplayInfo display);


    /**
        Changes the text variable but nothing changes visually unless the update functions is called.

        @param<text> is the new value for @variable<text>
    */
    void setTextMessage(TextInfo text);


    /**
        @return @variable<display>
    */
    DisplayInfo getDisplay() const;


    /**
        @return @variable<text>
    */
    TextInfo getTextMessage() const;


private:
    DisplayInfo display;
    TextInfo text;
    QString styleSheetUrl;
};


#endif // PUSHBUTTON_H
