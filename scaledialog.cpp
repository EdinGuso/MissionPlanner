/**
    MissionPlanner
    scaledialog.cpp
    Purpose: ScaleDialog class implementation

    @author Edin Guso
    @version 1.0
    @date 26/07/2019
*/


#include "scaledialog.h"


ScaleDialog::ScaleDialog(QImage img, double &pixelLength, double &realLength, QPoint &pixelOrigin,
                         QSize displaySize, QSize screenSize) : QDialog(), numClicks(0), pixelLength(pixelLength),
    realLength(realLength), pixelOrigin(pixelOrigin), displaySize(displaySize), screenSize(screenSize) {

    setWindowState(Qt::WindowMaximized);

    QPixmap pix = QPixmap::fromImage(img);
    pix = pix.scaled(screenSize.width(), screenSize.height(), Qt::KeepAspectRatio);
    img = pix.toImage();
    originalImg = img;

    mapDisplay = new ImageLabel(
                DisplayInfo(
                    QPoint(0, 0),
                    QSize(img.width(), img.height())),
                img,
                this);

    connect(mapDisplay, &ImageLabel::mouseLeftClick, this, &ScaleDialog::mouseLeftClick);
    connect(mapDisplay, &ImageLabel::mouseRightClick, this, &ScaleDialog::mouseRightClick);

    ///required in order to show the map before the messagebox popup
    this->show();

    QMessageBox scaleMessageBox(QMessageBox::Information, "Scale Setup",
                                "Please select two points on the map to select a section by left clicking on the map.\n"
                                "You can cancel your first click by right clicking anywhere on the map.",
                                QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
    scaleMessageBox.exec();

    ratio = double(displaySize.width()) / double(mapDisplay->size().width());
}


void ScaleDialog::mouseLeftClick(QPoint mousePos) {
    const int PEN_WIDTH = 5;
    const double CIRCLE_RADIUS = 5;
    if(numClicks < 2) {
        QPixmap pix = *(mapDisplay->pixmap());
        QImage img = pix.toImage();
        QPainter painter(&img);
        QPen pen(Qt::red);
        pen.setWidth(PEN_WIDTH);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(mousePos), CIRCLE_RADIUS, CIRCLE_RADIUS);
        mapDisplay->setPixmap(QPixmap::fromImage(img));

        if(numClicks == 0)
            firstClick = mousePos;
        else if(numClicks == 1) {
            QLineF line(firstClick, mousePos);
            QPixmap pix = *(mapDisplay->pixmap());
            QImage img = pix.toImage();
            QPainter painter(&img);
            QPen pen(Qt::red);
            pen.setWidth(PEN_WIDTH);
            painter.setPen(pen);
            painter.drawLine(line);
            mapDisplay->setPixmap(QPixmap::fromImage(img));

            pixelLength = line.length() * ratio;

            realLength = 0;
            while(realLength == 0) {
                QInputDialog inputDialog;
                realLength = inputDialog.getDouble(this, "Input",
                                                   "Please input the real length of the selected section in metres.\n",
                                                   0, 0, INT_MAX, 3, nullptr, Qt::MSWindowsFixedSizeDialogHint);
            }
            QMessageBox originMessageBox(QMessageBox::Information, "Origin Setup",
                                         "Please select a point on the map to select the origin",
                                         QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
            originMessageBox.exec();
        }
        numClicks++;
    }
    else {
        pixelOrigin = QPoint(int(double(mousePos.x()) * ratio), int(double(mousePos.y()) * ratio));
        done(1);
    }
}


void ScaleDialog::mouseRightClick(QPoint mousePos) {
    if (numClicks < 2) {
        numClicks = 0;
        mapDisplay->setPixmap(QPixmap::fromImage(originalImg));
    }
}
