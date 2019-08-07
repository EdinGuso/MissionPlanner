/**
    MissionPlanner
    mapwindow.h
    Purpose: MapWindow class declaration

    @author Edin Guso
    @version 1.0
    @date 17/07/2019
*/


#ifndef MAPWINDOW_H
#define MAPWINDOW_H


#include <QMainWindow>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QStack>
#include <QTextStream>
#include <QProgressDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include "imagelabel.h"
#include "textlabel.h"
#include "pushbutton.h"
#include "checkbox.h"
#include "supportstructs.h"
#include "vehicle.h"
#include "rover.h"
#include "quad.h"
#include "fwuav.h"
#include "shape.h"
#include "rectangle.h"
#include "circle.h"
#include "polygon.h"
#include "reachpoints.h"
#include "vehicleinputdialog.h"


namespace Ui {
class MapWindow;
}


/**
    @class MapWindow
    @inherits QMainWindow
    Handles everything that is happening on the mapwindow

    @variable<ui> pointer to the designer form (will need to be used if anyone uses the designer form)
    @variable<mapScroll> deals with scrolling and resizing of the map
    @variable<mapDisplay> detects all the user clicks on the map and displays the map
    @variable<previousWindow> stores a pointer to the main window so that it can be displayed later if needed
    @variable<originalImage> stores the original map with its original size
    @variable<zoomAmount> stores the current zoom amount of the map
    @variable<menuNumber> stores the menu the user is in (vehicle selection menu etc.)
    @variable<saveDirectory> stores the directory where the project will be saved (empty if not saved yet)
    @variable<pixelLength> stores the pixel length of the selected section
    @variable<realLength> stores the real length of the selected section
    @variable<pixelOrigin> stores the pixel position of the selected origin point
    @variable<vehicleButton> pointer to the push button
    @variable<missionButton> pointer to the push button
    @variable<exitButton> pointer to the push button
    @variable<roverButton> pointer to the push button
    @variable<quadButton> pointer to the push button
    @variable<fwuavButton> pointer to the push button
    @variable<missionFromVehicleButton> pointer to the push button
    @variable<reachBox> pointer to the check box
    @variable<patrolBox> pointer to the check box
    @variable<noflyBox> pointer to the check box
    @variable<rectangleBox> pointer to the check box
    @variable<circleBox> pointer to the check box
    @variable<polygonBox> pointer to the check box
    @variable<backButton> pointer to the push button
    @variable<vehicleOneBox> pointer to the check box
    @variable<vehicleTwoBox> pointer to the check box
    @variable<vehicleThreeBox> pointer to the check box
    @variable<vehicleFourBox> pointer to the check box
    @variable<vehicleFiveBox> pointer to the check box
    @variable<menuBarHelp> pointer to the text label
    @variable<vehicleBoxesHelp> pointer to the text label
    @variable<exitButtonHelp> pointer to the text label
    @variable<vehicleButtonHelp> pointer to the text label
    @variable<missionButtonHelp> pointer to the text label
    @variable<roverButtonHelp> pointer to the text label
    @variable<quadButtonHelp> pointer to the text label
    @variable<fwuavButtonHelp> pointer to the text label
    @variable<missionFromVehicleButtonHelp> pointer to the text label
    @variable<backButtonHelpVehicle> pointer to the text label
    @variable<reachBoxHelp> pointer to the text label
    @variable<patrolBoxHelp> pointer to the text label
    @variable<noflyBoxHelp> pointer to the text label
    @variable<rectangleBoxHelp> pointer to the text label
    @variable<circleBoxHelp> pointer to the text label
    @variable<polygonBoxHelp> pointer to the text label
    @variable<backButtonHelpMission> pointer to the text label
    @variable<editHelp> pointer to the text label
    @variable<saveAct> pointer to the action
    @variable<saveAsAct> pointer to the action
    @variable<exitAct> pointer to the action
    @variable<undoAct> pointer to the action
    @variable<zoomInAct> pointer to the action
    @variable<zoomOutAct> pointer to the action
    @variable<deleteAct> pointer to the action
    @variable<helpAct> pointer to the action
    @variable<vehicleAct> pointer to the action
    @variable<missionAct> pointer to the action
    @variable<editAct> pointer to the action...
    @variable<backAct> pointer to the action
    @variable<vehicleOneAct> pointer to the action
    @variable<vehicleTwoAct> pointer to the action
    @variable<vehicleThreeAct> pointer to the action
    @variable<vehicleFourAct> pointer to the action
    @variable<vehicleFiveAct> pointer to the action
    @variable<fileMenu> pointer to the menu
    @variable<toolMenu> pointer to the menu
    @variable<menuMenu> pointer to the menu
    @variable<vehicleMenu> pointer to the menu
    @variable<vehicleClicked> stores which type of vehicle has been clicked in the vehicle menu
    @variable<previousClicks> stores the previous clicks when drawing in the mission menu
    @variable<cureentMission> stores the selected mission when drawing in the mission menu
    @variable<selectedVehicle> stores the selected vehicle when drawing in the mission menu
    @variable<toBeDeletedVehicle> stores the vehicle that is selected for deletion
    @variable<toBeDeletedShape> stores the shape that is selected for deletion
    @variable<vehicleOrShape> stores whether a vehicle or a shape is selected for deletion
    @variable<nextVehicleIndex> variable used for cycling through vehicles that are on top of each other
             (when clicking them to select)
    @variable<nextShapeIndex> variable used for cycling through shapes that are on top of each other
             (when clicking them to select)
    @variable<availableVehicles> stores which vehicles are currently on the field
    @variable<vehicleAmount> stores the amount of vehicles currently on the field
    @variable<isInEdit> stores whether the user is in edit menu or not
    @variable<toBeEditedVehicle> stores which vehicle is selected for editing
    @variable<toBeEditedShape> stores which shape is selected for editing
    @variable<isInHelp> stores whether the user has turned on help or not
    @variable<operationStack> stores the previous operations (delete/add) to know what to do for undo
    @variable<vehicleStack> stores deleted vehicles in a stack so that they can be replaced with undo
    @variable<shapeStack> stores deleted shapes in a stack so that they can be replaced with undo
    @variable<ongoingUndo> stores whether current operation is undo or not
    @variable<roverImage> stores a rover image
    @variable<quadImage> stores a quad image
    @variable<fwuavImage> stores a fwuav image
    @variable<vehicles> stores pointers to all the vehicles that are currently on the field
*/
class MapWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
        Constructor

        @param<image> map image uploaded by user
        @param<parent> pointer to the parent of the object, used to return to the welcome window
    */
    explicit MapWindow(QImage image = QImage(), QWidget *parent = nullptr);


    /**
        Constructor

        @param<directoryName> folder name from which the mapWindow object will be created
        @param<parent> pointer to the parent of the object, used to return to the welcome window
    */
    explicit MapWindow(QString directoryName, QWidget *parent = nullptr);


    /**
        Destructor
    */
    ~MapWindow();


    /**
        Changes the pixel length variable

        @param<pixelLength> is the new value for @variable<pixelLength>
    */
    void setPixelLength(double pixelLength);


    /**
        Changes the real length variable

        @param<realLength> is the new value for @variable<realLength>
    */
    void setRealLength(double realLength);


    /**
        Changes the pixel origin variable

        @param<pixelOrigin> is the new value for @variable<pixelOrigin>
    */
    void setPixelOrigin(QPoint pixelOrigin);


    /**
        @return @variable<mapDisplay>
    */
    ImageLabel* getMapDisplay();


private slots:
    /**
        Recieves the mouse left click signal from mapDisplay and handles it

        @param<mousePos> is the local position of the cursor
    */
    void mouseLeftClick(QPoint mousePos);


    /**
        Recieves the mouse right click signal from mapDisplay and handles it

        @param<mousePos> is the local position of the cursor
    */
    void mouseRightClick(QPoint mousePos);


    /**
        Recieves the mouse wheel forward signal from mapDisplay and handles it

        @param<mousePos> is the local position of the cursor
    */
    void zoomIn(QPoint mousePos);


    /**
        Recieves the mouse wheel backward signal from mapDisplay and handles it

        @param<mousePos> is the local position of the cursor
    */
    void zoomOut(QPoint mousePos);


    /**
        Recieves the mouse wheel forward signal from the keyboard shortcut and handles it
    */
    void zoomInShortcut();


    /**
        Recieves the mouse wheel backward signal from the keyboard shortcut and handles it
    */
    void zoomOutShortcut();


    /**
        Recieves the save signal from the keyboard shortcut and handles it
    */
    void saveActDone();


    /**
        Recieves the save as signal from the keyboard shortcut and handles it
    */
    void saveAsActDone();


    /**
        Recieves the undo signal from the keyboard shortcut and handles it
    */
    void undoActDone();


    /**
        Recieves the exit button clicked signal and handles it
    */
    void exitButtonClicked();


    /**
        Recieves the vehicle button clicked signal and handles it
    */
    void vehicleButtonClicked();


    /**
        Recieves the mission button clicked signal and handles it
    */
    void missionButtonClicked();


    /**
        Recieves the edit signal from the keyboard shortcut and handles it
    */
    void editActDone();


    /**
        Recieves the rover button clicked signal and handles it
    */
    void roverButtonClicked();


    /**
        Recieves the quad button clicked signal and handles it
    */
    void quadButtonClicked();


    /**
        Recieves the fwuav button clicked signal and handles it
    */
    void fwuavButtonClicked();


    /**
        Recieves the reach box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void reachBoxClicked(int state);


    /**
        Recieves the patrol box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void patrolBoxClicked(int state);


    /**
        Recieves the nofly box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void noflyBoxClicked(int state);


    /**
        Recieves the rectangle box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void rectangleBoxClicked(int state);


    /**
        Recieves the circle box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void circleBoxClicked(int state);


    /**
        Recieves the polygon box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void polygonBoxClicked(int state);


    /**
        Recieves the back button clicked signal and handles it
    */
    void backButtonClicked();


    /**
        Recieves the vehicle one box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void vehicleOneBoxClicked(int state);


    /**
        Recieves the vehicle two box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void vehicleTwoBoxClicked(int state);


    /**
        Recieves the vehicle three box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void vehicleThreeBoxClicked(int state);


    /**
        Recieves the vehicle four box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void vehicleFourBoxClicked(int state);


    /**
        Recieves the vehicle five box clicked signal and handles it

        @param<state> determines if check box has been checked or unchecked
    */
    void vehicleFiveBoxClicked(int state);


    /**
        Recieves the vehicle one signal from the keyboard shortcut and handles it
    */
    void vehicleOneActDone();


    /**
        Recieves the vehicle two signal from the keyboard shortcut and handles it
    */
    void vehicleTwoActDone();


    /**
        Recieves the vehicle three signal from the keyboard shortcut and handles it
    */
    void vehicleThreeActDone();


    /**
        Recieves the vehicle four signal from the keyboard shortcut and handles it
    */
    void vehicleFourActDone();


    /**
        Recieves the vehicle five signal from the keyboard shortcut and handles it
    */
    void vehicleFiveActDone();


    /**
        Recieves the delete signal from the keyboard shortcut and handles it
    */
    void deleteActDone();


    /**
        Recieves the help signal from the keyboard shortcut and handles it
    */
    void helpActDone();


private:
    /**
        Sets up all the vehicle images
    */
    void setupImages();


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


    /**
        Sets up all the vehicles and their shapes and draws them onto the map from the given file

        @param<dataFilename> filename where the data is stored (a .qmp file)
    */
    void setupVehiclesAndMissionsFromFile(QString dataFilename);


    /**
        Resizes the map by the given factor

        @param<factor> amount of zoom in/out that will be done
        @param<point> point around which zoom in/out will be done
    */
    void scaleImage(double factor, QPoint point);


    /**
        Adjusts the scrollbars after the zoom in/out

        @param<scrollBar> pointer to the scroll bar
        @param<factor> amount of zoom in/out that will be done
        @param<mousePos> point around which zoom in/out will be done
    */
    void adjustScrollBar(QScrollBar *scrollBar, double factor, int mousePos = 0);


    /**
        Places the selected vehicle on the clicked point

        @param<mousePos> the cursor location
    */
    void vehiclePlacement(QPoint mousePos);


    /**
        Configures the mission on the clicked point (this can be start or end of the drawing)

        @param<mousePos> the cursor location
    */
    void missionConfiguration(QPoint mousePos);


    /**
        Finds which object (vehicle or shape) has been clicked on the map

        @param<mousePos> the cursor location
    */
    void whichObjectIsPressed(QPoint mousePos);


    /**
        Highlights the selected vehicle or shape
    */
    void highlight();


    /**
        Outlines the selected vehicle

        @param<selected> pointer to the selected vehicle
    */
    void outline(Vehicle *selected);


    /**
        Deletes the selected vehicle
    */
    void deleteVechicle();


    /**
        Deletes the selected shape
    */
    void deleteShape();


    /**
        Handles the click and move operation in the edit menu

        @param<mousePos> the cursor location
    */
    void clickAndMove(QPoint mousePos);


    /**
        Since vehicles do not necessarily have to be placed to the vector in order, there can be blanks.
        This function finds the real position of the desired vehicle

        @param<index> desired vehicle index
    */
    int findRealIndex(int index);


    /**
        Draws a marker on the given point

        @param<mousePos> the cursor location
    */
    void drawMarker(QPoint mousePos);


    /**
        Redraws all the vehicles and shapes
    */
    void redraw();


    /**
        Redraws all the vehicles and shapes, excluding the given vehicle

        @param<vehicle> pointer to the vehicle that will not be drawn
    */
    void redrawWithoutVehicle(Vehicle *vehicle);


    /**
        Redraws all the vehicles and shapes, excluding the given shape

        @param<shape> pointer to the shape that will not be drawn
    */
    void redrawWithoutShape(Shape *shape);


    Ui::MapWindow *ui;
    QScrollArea *mapScroll;
    ImageLabel *mapDisplay;
    QWidget *previousWindow;
    QImage originalImage;
    double zoomAmount;
    int menuNumber;
    QString saveDirectory;

    ///scale information
    double pixelLength;
    double realLength;
    QPoint pixelOrigin;

    ///right ribbon
    PushButton *vehicleButton;
    PushButton *missionButton;
    PushButton *exitButton;
    PushButton *roverButton;
    PushButton *quadButton;
    PushButton *fwuavButton;
    PushButton *missionFromVehicleButton;
    CheckBox *reachBox;
    CheckBox *patrolBox;
    CheckBox *noflyBox;
    CheckBox *rectangleBox;
    CheckBox *circleBox;
    CheckBox *polygonBox;
    PushButton *backButton;

    ///bottom ribbon
    CheckBox *vehicleOneBox;
    CheckBox *vehicleTwoBox;
    CheckBox *vehicleThreeBox;
    CheckBox *vehicleFourBox;
    CheckBox *vehicleFiveBox;

    ///help texts
    TextLabel *menuBarHelp;
    TextLabel *vehicleBoxesHelp;
    TextLabel *exitButtonHelp;

    TextLabel *vehicleButtonHelp;
    TextLabel *missionButtonHelp;

    TextLabel *roverButtonHelp;
    TextLabel *quadButtonHelp;
    TextLabel *fwuavButtonHelp;
    TextLabel *missionFromVehicleButtonHelp;
    TextLabel *backButtonHelpVehicle;

    TextLabel *reachBoxHelp;
    TextLabel *patrolBoxHelp;
    TextLabel *noflyBoxHelp;
    TextLabel *rectangleBoxHelp;
    TextLabel *circleBoxHelp;
    TextLabel *polygonBoxHelp;
    TextLabel *backButtonHelpMission;

    TextLabel *editHelp;

    ///file menu
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    ///tool menu
    QAction *undoAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *deleteAct;
    QAction *helpAct;

    ///menu menu
    QAction *vehicleAct;
    QAction *missionAct;
    QAction *editAct;
    QAction *backAct;

    ///vehicle menu
    QAction *vehicleOneAct;
    QAction *vehicleTwoAct;
    QAction *vehicleThreeAct;
    QAction *vehicleFourAct;
    QAction *vehicleFiveAct;

    ///menubar
    QMenu *fileMenu;
    QMenu *toolMenu;
    QMenu *menuMenu;
    QMenu *vehicleMenu;

    int vehicleClicked;
    QVector<QPointF> previousClicks;
    missionType currentMission;
    Vehicle *selectedVehicle;
    Vehicle *toBeDeletedVehicle;
    Shape *toBeDeletedShape;
    bool vehicleOrShape;
    int nextVehicleIndex;
    int nextShapeIndex;
    bool availableVehicles[5];
    int vehicleAmount;
    bool isInEdit;
    Vehicle *toBeEditedVehicle;
    Shape *toBeEditedShape;
    bool isInHelp;

    QStack<operation> operationStack;
    QStack<Vehicle*> vehicleStack;
    QStack<Shape*> shapeStack;
    bool ongoingUndo;

    QPixmap roverImage;
    QPixmap quadImage;
    QPixmap fwuavImage;

    QVector<Vehicle*> vehicles;
};


#endif // MAPWINDOW_H
