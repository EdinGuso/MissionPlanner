/**
    MissionPlanner
    imagelabel.cpp
    Purpose: ImageLabel class implementation

    @author Edin Guso
    @version 1.0
    @date 17/07/2019
*/


#include "imagelabel.h"


ImageLabel::ImageLabel(DisplayInfo display, QImage image, QWidget *parent) : QLabel(parent), display(display) {
    updateLabel();
    setPixmap(QPixmap::fromImage(image));
}


void ImageLabel::updateLabel() {
    move(display.pos);
    resize(display.size);
}


void ImageLabel::setDisplay(DisplayInfo display) {
    this->display = display;
}


DisplayInfo ImageLabel::getDisplay() const {
    return display;
}

void ImageLabel::wheelEvent(QWheelEvent *event) {
    QCursor cursor;
    QPoint globalPoint = cursor.pos();
    QPoint point = mapFromGlobal(globalPoint);
    ///if the mousewheel is scrolled forward
    if(event->delta() > 0)
        emit zoomIn(point);
    ///if the mousewheel is scrolled backward
    else
        emit zoomOut(point);
}


void ImageLabel::mousePressEvent(QMouseEvent *event) {
    QCursor cursor;
    QPoint globalPoint = cursor.pos();
    QPoint point = mapFromGlobal(globalPoint);
    if (event->buttons() == Qt::LeftButton)
        emit mouseLeftClick(point);
    else if(event->buttons() == Qt::RightButton)
        emit mouseRightClick(point);
}

