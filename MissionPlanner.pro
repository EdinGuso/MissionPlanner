#-------------------------------------------------
#
# Project created by QtCreator 2019-07-16T10:53:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MissionPlanner
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        checkbox.cpp \
        circle.cpp \
        fwuav.cpp \
        imagelabel.cpp \
        main.cpp \
        mapwindow.cpp \
        polygon.cpp \
        polygondetection.cpp \
        pushbutton.cpp \
        quad.cpp \
        reachpoints.cpp \
        rectangle.cpp \
        rover.cpp \
        scaledialog.cpp \
        shape.cpp \
        textlabel.cpp \
        vehicle.cpp \
        vehicleinputdialog.cpp \
        welcomewindow.cpp

HEADERS += \
        checkbox.h \
        circle.h \
        fwuav.h \
        imagelabel.h \
        mapwindow.h \
        polygon.h \
        polygondetection.h \
        pushbutton.h \
        quad.h \
        reachpoints.h \
        rectangle.h \
        rover.h \
        scaledialog.h \
        shape.h \
        supportstructs.h \
        textlabel.h \
        vehicle.h \
        vehicleinputdialog.h \
        welcomewindow.h

FORMS += \
        mapwindow.ui \
        welcomewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
