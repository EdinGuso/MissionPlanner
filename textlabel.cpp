/**
    MissionPlanner
    textlabel.cpp
    Purpose: TextLabel class implementation

    @author Edin Guso
    @version 1.0
    @date 16/07/2019
*/


#include "textlabel.h"


TextLabel::TextLabel(DisplayInfo display, TextInfo text, QWidget *parent) :
    QLabel(parent), display(display), text(text)  {
    updateLabel();
}


void TextLabel::updateLabel() {
    move(display.pos);
    resize(display.size);
    setText(text.message);
    setFont(text.font);
    setAlignment(Qt::AlignCenter);
}


void TextLabel::setDisplay(DisplayInfo display) {
    this->display = display;
}


void TextLabel::setTextMessage(TextInfo text) {
    this->text = text;
}


DisplayInfo TextLabel::getDisplay() const {
    return display;
}


TextInfo TextLabel::getTextMessage() const {
    return text;
}
