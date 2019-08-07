/**
    MissionPlanner
    welcomewindow.h
    Purpose: WelcomeWindow class declaration

    @author Edin Guso
    @version 1.0
    @date 16/07/2019
*/


#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H


#include <QMainWindow>
#include <QScreen>
#include <QFileDialog>
#include <QMenu>
#include <QStandardPaths>
#include "textlabel.h"
#include "pushbutton.h"
#include "scaledialog.h"
#include "mapwindow.h"


namespace Ui {
class WelcomeWindow;
}


/**
    @class WelcomeWindow
    @inherits QMainWindow
    Handles everything that is happening on the welcome window

    @variable<ui> pointer to the designer form (will need to be used if anyone uses the designer form)
    @variable<titleText> displays the program name
    @variable<openProjectButton> connected to the open project slot
    @variable<newProjectButton> connected to the new project slot
    @variable<descriptionText> displays the program description
    @variable<newProjectAct> connects the menubar click and keyboard shortcut to new project slot
    @variable<openProjectAct> connects the menubar click and keyboard shortcut to open project slot
    @variable<fileMenu> displays the menubar
    @variable<nextWindow> holds a pointer to the next window
    @variable<pixelLength> stores the pixel length of the selected section
    @variable<realLength> stores the real length of the selected section
    @variable<pixelOrigin> stores the pixel position of the selected origin point
*/
class WelcomeWindow : public QMainWindow {
    Q_OBJECT
public:
    /**
        Constructor

        @param<parent> pointer to the parent of the object, passed onto the @class<QMainWindow> constructor
    */
    explicit WelcomeWindow(QWidget *parent = nullptr);


    /**
        Destructor
    */
    ~WelcomeWindow();


private slots:
    /**
        Handles all the things needed for project opening such as asking for a folder name,
        checking its contents, creating the mapWindow object etc.
    */
    void openProjectClicked();


    /**
        Handles all the things needed for project creation such as asking for a map file name,
        displaying the scale dialog popup, checking if the inputs have been correctly entered etc.
    */
    void newProjectClicked();


    /**
        If the scale inputs were properly entered and the process was not halted by the user,
        creats the mapWindow, displays it and hides the current window
    */
    void scaleInputsAccepted();


private:
    /**
        Sets up all the widgets with correct sizes for the screen size
    */
    void setupWidgets();


    /**
        Sets up all the actions and their shortcuts
    */
    void setupActions();


    /**
        Sets up all the menus in the menubar and assigns actions to them
    */
    void setupMenus();


    /**
        Sets up all the connections
    */
    void setupConnections();


    Ui::WelcomeWindow *ui;
    TextLabel *titleText;
    PushButton *openProjectButton;
    PushButton *newProjectButton;
    TextLabel *descriptionText;
    QAction *newProjectAct;
    QAction *openProjectAct;
    QMenu *fileMenu;
    MapWindow *nextWindow;
    double pixelLength;
    double realLength;
    QPoint pixelOrigin;
};


#endif // WELCOMEWINDOW_H
