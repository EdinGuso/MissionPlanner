/**
    MissionPlanner
    pushbutton.cpp
    Purpose: PushButton class implementation

    @author Edin Guso
    @version 1.0
    @date 16/07/2019
*/


#include "pushbutton.h"


PushButton::PushButton(DisplayInfo display, TextInfo text, QWidget *parent, QString styleSheetUrl) :
    QPushButton(parent), display(display), text(text), styleSheetUrl(styleSheetUrl)  {
    updateButton();
}


void PushButton::updateButton(){
    move(display.pos);
    resize(display.size);
    setText(text.message);
    setFont(text.font);
    ///if a string has been passed as an argument, the button gets the background and a new texture
    if(styleSheetUrl != "")
        setStyleSheet("QPushButton{ image: url(" + styleSheetUrl + ") }"
                      "QPushButton{ background-color: lightgrey; } "
                      "QPushButton:hover{ background-color: silver; }"
                      "QPushButton:focus:hover{ background-color: salmon }"
                      "QPushButton:disabled{ background-color: dimgray }");
}


void PushButton::setDisplay(DisplayInfo display){
    this->display = display;
}


void PushButton::setTextMessage(TextInfo text){
    this->text = text;
}


DisplayInfo PushButton::getDisplay() const{
    return display;
}


TextInfo PushButton::getTextMessage() const{
    return text;
}
