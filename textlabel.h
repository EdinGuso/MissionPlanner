/**
    MissionPlanner
    textlabel.h
    Purpose: TextLabel class declaration

    @author Edin Guso
    @version 1.0
    @date 16/07/2019
*/


#ifndef TEXTLABEL_H
#define TEXTLABEL_H


#include <QLabel>
#include "supportstructs.h"


/**
    @class TextLabel
    @inherits QLabel
    Easier to setup the label with text display. Instead of
    having to call several functions after the constructor
    in QLabel, you can call the constructor itself to
    setup the label with a text display

    @variable<display> stores information about widget position and size
    @variable<text> stores information about text content and font
*/
class TextLabel : public QLabel {
public:
    /**
        Constructor that can be called with no parameters as well as
        more parameters. It initializes all the variables

        @param<display> initializes @variable<display>
        @param<text> initializes @variable<text>
        @param<parent> ointer to the parent of the object, passed onto the @class<QLabel> constructor
    */
    TextLabel(DisplayInfo display = DisplayInfo(), TextInfo text = TextInfo(), QWidget *parent = nullptr);


    /**
        Moves, resizes and sets all the text and stylesheet options for the checkbox. This function is
        also called form the construction for the initial setup.
    */
    void updateLabel();


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
};


#endif // TEXTLABEL_H
