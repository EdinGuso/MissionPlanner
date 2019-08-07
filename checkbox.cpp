/**
    MissionPlanner
    checkbox.cpp
    Purpose: CheckBox class implementation

    @author Edin Guso
    @version 1.0
    @date 22/07/2019
*/


#include "checkbox.h"


CheckBox::CheckBox(DisplayInfo display, TextInfo text, QWidget *parent, QString styleSheetUrl) :
    QCheckBox(parent), display(display), text(text), styleSheetUrl(styleSheetUrl) {
    updateButton();
}


void CheckBox::updateButton() {
    move(display.pos);
    resize(display.size);
    setText(text.message);
    setFont(text.font);
    ///if the keyword "a" has been passed as an argument, then the checkbox is a regular checkbox
    if(styleSheetUrl == "a")
        setStyleSheet("QCheckBox{ image: url(" + styleSheetUrl + ") }"
                      "QCheckBox:unchecked{ background-color: lightgrey; } "
                      "QCheckBox:unchecked:hover{ background-color: silver; }"
                      "QCheckBox:checked{ background-color: tomato; }"
                      "QCheckBox:checked:hover{ background-color: salmon }"
                      "QCheckBox:disabled{ background-color: dimgray }");

    ///otherwise it is a checkbox with the indicator filling all the area
    else if(styleSheetUrl != "")
        setStyleSheet("QCheckBox::indicator { width: " + QString::number(display.size.width()) + "px;"
                      "height: " + QString::number(display.size.height()) + "px; }"
                      "QCheckBox::indicator{ image: url(" + styleSheetUrl + ") }"
                      "QCheckBox::indicator:unchecked{ background-color: lightgrey; } "
                      "QCheckBox::indicator:unchecked:hover{ background-color: silver; }"
                      "QCheckBox::indicator:checked{ background-color: tomato; }"
                      "QCheckBox::indicator:checked:hover{ background-color: salmon }"
                      "QCheckBox::indicator:disabled{ background-color: dimgray }");

}


void CheckBox::setDisplay(DisplayInfo display) {
    this->display = display;
}


void CheckBox::setTextMessage(TextInfo text ){
    this->text = text;
}


DisplayInfo CheckBox::getDisplay() const {
    return display;
}


TextInfo CheckBox::getTextMessage() const {
    return text;
}
