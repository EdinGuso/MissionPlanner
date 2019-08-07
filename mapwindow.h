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
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
    @variable<...> ...
*/
class MapWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MapWindow(QImage image = QImage(), QWidget *parent = nullptr);
    explicit MapWindow(QString directoryName, QWidget *parent = nullptr);
    ~MapWindow();

    void setPixelLength(double pixelLength);
    void setRealLength(double realLength);
    void setPixelOrigin(QPoint pixelOrigin);
    ImageLabel* getMapDisplay();

private slots:
    void mouseLeftClick(QPoint mousePos);
    void mouseRightClick(QPoint mousePos);
    void zoomIn(QPoint mousePos);
    void zoomOut(QPoint mousePos);
    void zoomInShortcut();
    void zoomOutShortcut();

    void saveActDone();
    void saveAsActDone();

    void undoActDone();
    void exitButtonClicked();
    void vehicleButtonClicked();
    void missionButtonClicked();
    void editActDone();
    void roverButtonClicked();
    void quadButtonClicked();
    void fwuavButtonClicked();
    void reachBoxClicked(int state);
    void patrolBoxClicked(int state);
    void noflyBoxClicked(int state);
    void rectangleBoxClicked(int state);
    void circleBoxClicked(int state);
    void polygonBoxClicked(int state);
    void backButtonClicked();

    void vehicleOneBoxClicked(int state);
    void vehicleTwoBoxClicked(int state);
    void vehicleThreeBoxClicked(int state);
    void vehicleFourBoxClicked(int state);
    void vehicleFiveBoxClicked(int state);
    void vehicleOneActDone();
    void vehicleTwoActDone();
    void vehicleThreeActDone();
    void vehicleFourActDone();
    void vehicleFiveActDone();

    void deleteActDone();
    void helpActDone();

private:
    void setupImages();
    void setupWidgets();
    void setupActions();
    void setupMenus();
    void setupConnections();
    void setupVehiclesAndMissionsFromFile(QString dataFilename);
    void scaleImage(double factor, QPoint point);
    void adjustScrollBar(QScrollBar *scrollBar, double factor, int mousePos = 0);
    void vehiclePlacement(QPoint mousePos);
    void missionConfiguration(QPoint mousePos);
    void whichObjectIsPressed(QPoint mousePos);
    void highlight();
    void outline(Vehicle *selected);
    void deleteVechicle();
    void deleteShape();
    void clickAndMove(QPoint mousePos);
    int findRealIndex(int index);
    void drawMarker(QPoint mousePos);
    void redraw();
    void redrawWithoutVehicle(Vehicle *vehicle);
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
    Vehicle *selectedVehicle;   ///selected for  mission configuration
    Vehicle *toBeDeletedVehicle;///selected for deletion
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
