/**
    MissionPlanner
    mapwindow.cpp
    Purpose: MapWindow class implementation

    @author Edin Guso
    @version 1.0
    @date 17/07/2019
*/


#include "mapwindow.h"
#include "ui_mapwindow.h"

MapWindow::MapWindow(QImage image, QWidget *parent) : QMainWindow(), ui(new Ui::MapWindow),
    previousWindow(parent), originalImage(image), zoomAmount(1), menuNumber(0), saveDirectory(""),
    vehicleClicked(0), currentMission(NoMission), selectedVehicle(nullptr), vehicleOrShape(false),
    nextVehicleIndex(0), nextShapeIndex(0), availableVehicles{true, true, true , true, true},
    vehicleAmount(0), isInEdit(false), isInHelp(false), ongoingUndo(false),
    vehicles(QVector<Vehicle*>(5)) {

    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setupImages();
    setupWidgets();
    setupActions();
    setupMenus();
    setupConnections();
}


MapWindow::MapWindow(QString directoryName, QWidget *parent) : QMainWindow(),
    ui(new Ui::MapWindow), previousWindow(parent), zoomAmount(1), menuNumber(0),
    vehicleClicked(0), currentMission(NoMission), selectedVehicle(nullptr), vehicleOrShape(false),
    nextVehicleIndex(0), nextShapeIndex(0), isInEdit(false), isInHelp(false), ongoingUndo(false),
    vehicles(QVector<Vehicle*>(5)) {

    saveDirectory = directoryName;
    QString imageFilename = directoryName + "/original_image.vmp";
    QString dataFilename = directoryName + "/data.qmp";
    originalImage = QImage(imageFilename);

    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setupImages();
    setupWidgets();
    setupActions();
    setupMenus();
    setupConnections();
    setupVehiclesAndMissionsFromFile(dataFilename);

    redraw();
}


MapWindow::~MapWindow() {
    delete ui;
    delete mapScroll;
    ///delete mapDisplay; cannot delete mapDisplay since it is already deleted when mapscroll is deleted
    delete vehicleButton;
    delete missionButton;
    delete exitButton;
    delete roverButton;
    delete quadButton;
    delete fwuavButton;
    delete missionFromVehicleButton;
    delete reachBox;
    delete patrolBox;
    delete noflyBox;
    delete rectangleBox;
    delete circleBox;
    delete polygonBox;
    delete backButton;
    delete vehicleOneBox;
    delete vehicleTwoBox;
    delete vehicleThreeBox;
    delete vehicleFourBox;
    delete vehicleFiveBox;
    delete menuBarHelp;
    delete vehicleBoxesHelp;
    delete exitButtonHelp;
    delete vehicleButtonHelp;
    delete missionButtonHelp;
    delete roverButtonHelp;
    delete quadButtonHelp;
    delete fwuavButtonHelp;
    delete missionFromVehicleButtonHelp;
    delete backButtonHelpVehicle;
    delete reachBoxHelp;
    delete patrolBoxHelp;
    delete noflyBoxHelp;
    delete rectangleBoxHelp;
    delete circleBoxHelp;
    delete polygonBoxHelp;
    delete backButtonHelpMission;
    delete editHelp;
    delete saveAct;
    delete saveAsAct;
    delete exitAct;
    delete undoAct;
    delete zoomInAct;
    delete zoomOutAct;
    delete deleteAct;
    delete helpAct;
    delete vehicleAct;
    delete missionAct;
    delete editAct;
    delete backAct;
    delete vehicleOneAct;
    delete vehicleTwoAct;
    delete vehicleThreeAct;
    delete vehicleFourAct;
    delete vehicleFiveAct;
    delete fileMenu;
    delete toolMenu;
    delete menuMenu;
    delete vehicleMenu;
}


void MapWindow::setPixelLength(double pixelLength) {
    this->pixelLength = pixelLength;
}


void MapWindow::setRealLength(double realLength) {
    this->realLength = realLength;
}


void MapWindow::setPixelOrigin(QPoint pixelOrigin) {
    this->pixelOrigin = pixelOrigin;
}


ImageLabel* MapWindow::getMapDisplay() {
    return mapDisplay;
}


void MapWindow::mouseLeftClick(QPoint mousePos) {
    toBeDeletedVehicle = nullptr;
    toBeDeletedShape = nullptr;
    if(isInEdit) {
        clickAndMove(mousePos);
        return;
    }
    switch (menuNumber) {
    case 1:
        if(vehicleClicked)
            vehiclePlacement(mousePos);
        else
            whichObjectIsPressed(mousePos);
        break;
    case 2:
        if(selectedVehicle && currentMission != NoMission)
            missionConfiguration(mousePos);
        else
            whichObjectIsPressed(mousePos);
        break;
    default:
        whichObjectIsPressed(mousePos);
        break;
    }
}


void MapWindow::mouseRightClick(QPoint mousePos) {
    toBeDeletedVehicle = nullptr;
    toBeDeletedShape = nullptr;
    redraw();
    if(isInEdit) {
        previousClicks.clear();
        toBeEditedVehicle = nullptr;
        toBeEditedShape = nullptr;
        setCursor(Qt::ArrowCursor);
        return;
    }
    switch (menuNumber) {
    case 1:
        vehicleClicked = 0;
        setCursor(Qt::ArrowCursor);
        break;
    case 2:
        previousClicks.clear();
        break;
    default:
        break;
    }
}


void MapWindow::zoomIn(QPoint mousePos) {
    if(zoomAmount > 3)
        return;
    scaleImage(1.1, mousePos);
}


void MapWindow::zoomOut(QPoint mousePos) {
    if(zoomAmount < 0.2)
        return;
    scaleImage(0.9090909, mousePos);
}


void MapWindow::zoomInShortcut() {
    zoomIn(QPoint(mapDisplay->width() / 2, mapDisplay->height() / 2));
}


void MapWindow::zoomOutShortcut() {
    zoomOut(QPoint(mapDisplay->width() / 2, mapDisplay->height() / 2));
}


void MapWindow::saveActDone() {
    if(saveDirectory.isNull() || saveDirectory.isEmpty())
        saveAsActDone();
    if(saveDirectory.isNull() || saveDirectory.isEmpty())
        return;
    originalImage.save(saveDirectory + "/original_image.vmp", "png");
    QString dataFilename = saveDirectory + "/data.qmp";
    QFile file(dataFilename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "BEGIN_QMP_ENCODING" << endl
               << "Directory: " << saveDirectory << endl
               << "pixel: " << pixelLength << " real: " << realLength
               << " origin: " << pixelOrigin.x() << " " << pixelOrigin.y() << endl
               << "v_amount: " << vehicleAmount << endl
               << "v_available: " << availableVehicles[0] << " " << availableVehicles[1] << " "
               << availableVehicles[2] << " " << availableVehicles[3] << " " << availableVehicles[4]
               << endl << endl << "Control_Information:" << endl;
        for(int i = 0; i < vehicleAmount; i++) {
            Vehicle *vehicle = vehicles[findRealIndex(i)];
            stream << "Vehicle " << vehicle->getVehicleType() << " " <<
                      vehicle->getDisplayInfo().pos.x() + vehicle->getDisplayInfo().size.width() / 2 << " " <<
                      vehicle->getDisplayInfo().pos.y() + vehicle->getDisplayInfo().size.height() / 2 << " ";
            vehicle->printParameters(stream);
            stream << endl;
            QVector<Shape*>* shapes = vehicle->getShapes();
            for(int j = 0; j < shapes->size(); j++) {
                Shape *shape = shapes->at(j);
                stream << "Shape " << shape->getVehicleNumber() << " ";
                shape->printParameters(stream);
                stream << endl;
            }
        }
        stream << "END_QMP_ENCODING";
    }
    file.close();
}


void MapWindow::saveAsActDone() {
    QInputDialog inputDialog;
    QString folderName = inputDialog.getText(this, "Input",
                                       "Please input the folder name.\nOnly use letters and underscore characters.",
                                       QLineEdit::Normal, "Folder Name", nullptr, Qt::MSWindowsFixedSizeDialogHint, Qt::ImhLowercaseOnly);
    if(folderName.isNull() || folderName.isEmpty()) {
        QMessageBox errorMessageBox(QMessageBox::Information, "Canceled",
                                    "Folder selection has been canceled.",
                                    QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
        errorMessageBox.exec();
        return;
    }
    QRegExp re("\\w*");  // a digit (\d), zero or more times (*)
    if (!re.exactMatch(folderName)) {
        QMessageBox errorMessageBox(QMessageBox::Information, "Invalid Folder",
                                    "Entered folder name contains invalid characters.",
                                    QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
        errorMessageBox.exec();
        return;
    }
    QString pathName = QFileDialog::getExistingDirectory(this, "Select a folder",
                                                         QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(pathName.isNull() || pathName.isEmpty()) {
        QMessageBox errorMessageBox(QMessageBox::Information, "Canceled",
                                    "Folder selection has been canceled.",
                                    QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
        errorMessageBox.exec();
        return;
    }
    QString fullDirectoryName = pathName + "/" + folderName;
    QDir dir(fullDirectoryName);
    if(!dir.exists())
        dir.mkpath(".");
    else {
        QMessageBox errorMessageBox(QMessageBox::Information, "Folder Exists",
                                    "Entered folder already exists!",
                                    QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
        errorMessageBox.exec();
        return;
    }
    saveDirectory = fullDirectoryName;
    saveActDone();
}


void MapWindow::undoActDone() {
    if(operationStack.empty())
        return;

    ongoingUndo = true;
    switch (operationStack.pop()) {
    case VehiclePlaced: {
        toBeDeletedVehicle = vehicleStack.pop();
        int i = 0;
        for(; i < vehicleAmount; i++){
            if(vehicles[findRealIndex(i)] == toBeDeletedVehicle)
                break;
        }
        nextVehicleIndex = i + 1;
        deleteVechicle();
        break;
    }
    case ShapePlaced: {
        bool found = false;
        toBeDeletedShape = shapeStack.pop();
        int i = 0;
        for (; i < vehicleAmount && !found; i++) {
            QVector<Shape*>* shapes = vehicles[findRealIndex(i)]->getShapes();
            int j = 0;
            for (; j < shapes->size() && !found; j++) {
                if(shapes->at(j) == toBeDeletedShape) {
                    nextVehicleIndex = i + 1;
                    nextShapeIndex = j + 1;
                    found = true;
                }
            }
        }
        deleteShape();
        break;
    }
    case VehicleRemoved:
        vehiclePlacement(QPoint());
        break;
    case ShapeRemoved:
        missionConfiguration(QPoint());
        break;
    }
    ongoingUndo = false;
    previousClicks.clear();
}


void MapWindow::exitButtonClicked() {
    ///if the shortcut is pressed but the button is disabled
    if(!exitButton->isEnabled())
        return;

    QMessageBox messageBox;
    messageBox.setMinimumSize(640, 480);
    messageBox.setText("Are you sure you want to exit?");
    messageBox.setInformativeText("Your unsaved progress will be lost!");
    messageBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
    int ret = messageBox.exec();
    if(ret == QMessageBox::Cancel)
        return;

    previousWindow->show();
    delete this;
}


void MapWindow::vehicleButtonClicked() {
    ///if the shortcut is pressed but were not in main menu, catch it here
    if(menuNumber != 0)
        return;

    ///if the shortcut is pressed but the button is disabled
    if(!vehicleButton->isEnabled())
        return;

    const int BUTTON_OFFSET = 5;

    vehicleButton->hide();
    missionButton->hide();
    roverButton->show();
    quadButton->show();
    fwuavButton->show();
    missionFromVehicleButton->show();
    backButton->show();
    backButton->move(missionFromVehicleButton->pos().x(),
                     missionFromVehicleButton->pos().y() + missionFromVehicleButton->size().height() + BUTTON_OFFSET);
    menuNumber = 1;
    isInHelp = false;
    vehicleButtonHelp->hide();
    missionButtonHelp->hide();
    menuBarHelp->hide();
    vehicleBoxesHelp->hide();
    exitButtonHelp->hide();
}


void MapWindow::missionButtonClicked() {
    ///if the shortcut is pressed but no vehicle is placed yet or were in mission menu already, catch it here
    if(vehicleAmount == 0 || menuNumber == 2)
        return;

    ///if the shortcut is pressed but the button is disabled
    if(!missionButton->isEnabled())
        return;

    const int BUTTON_OFFSET = 5;

    if(menuNumber == 0) {
        vehicleButton->hide();
        missionButton->hide();
        vehicleButtonHelp->hide();
        missionButtonHelp->hide();
        menuBarHelp->hide();
        vehicleBoxesHelp->hide();
        exitButtonHelp->hide();
    }
    else if(menuNumber == 1) {
        roverButton->hide();
        quadButton->hide();
        fwuavButton->hide();
        missionFromVehicleButton->hide();
        roverButtonHelp->hide();
        quadButtonHelp->hide();
        fwuavButtonHelp->hide();
        missionFromVehicleButtonHelp->hide();
        backButtonHelpVehicle->hide();
        menuBarHelp->hide();
        vehicleBoxesHelp->hide();
        exitButtonHelp->hide();
        vehicleClicked = 0;
        setCursor(Qt::ArrowCursor);
    }
    reachBox->show();
    patrolBox->show();
    noflyBox->show();
    rectangleBox->show();
    circleBox->show();
    polygonBox->show();
    backButton->show();
    backButton->move(polygonBox->pos().x(),
                     polygonBox->pos().y() + polygonBox->size().height() + BUTTON_OFFSET);
    menuNumber = 2;
    isInHelp = false;
}


void MapWindow::editActDone() {
    ///there is nothing to edit
    if(vehicleAmount == 0)
        return;

    ///if we are in edit mode and leaving
    if(isInEdit) {
        setCursor(Qt::ArrowCursor);
        previousClicks.clear();
        editHelp->hide();
        isInHelp = false;
        vehicleOneBox->setEnabled(true);
        vehicleTwoBox->setEnabled(true);
        vehicleThreeBox->setEnabled(true);
        vehicleFourBox->setEnabled(true);
        vehicleFiveBox->setEnabled(true);
        backButton->setEnabled(true);
        exitButton->setEnabled(true);
        switch (menuNumber) {
        case 0:
            vehicleButton->setEnabled(true);
            missionButton->setEnabled(true);
            break;
        case 1:
            roverButton->setEnabled(true);
            quadButton->setEnabled(true);
            fwuavButton->setEnabled(true);
            missionFromVehicleButton->setEnabled(true);
            break;
        case 2:

            break;
        default:
            break;
        }
        redraw();
    }
    ///if we are not in edit mode and entering
    else {
        menuBarHelp->hide();
        vehicleBoxesHelp->hide();
        exitButtonHelp->hide();
        vehicleButtonHelp->hide();
        missionButtonHelp->hide();
        roverButtonHelp->hide();
        quadButtonHelp->hide();
        fwuavButtonHelp->hide();
        reachBoxHelp->hide();
        patrolBoxHelp->hide();
        noflyBoxHelp->hide();
        rectangleBoxHelp->hide();
        circleBoxHelp->hide();
        polygonBoxHelp->hide();
        backButtonHelpMission->hide();
        missionFromVehicleButtonHelp->hide();
        backButtonHelpVehicle->hide();
        isInHelp = false;
        toBeEditedVehicle = nullptr;
        toBeEditedShape = nullptr;
        vehicleOneBox->setEnabled(false);
        vehicleTwoBox->setEnabled(false);
        vehicleThreeBox->setEnabled(false);
        vehicleFourBox->setEnabled(false);
        vehicleFiveBox->setEnabled(false);
        vehicleOneBox->setChecked(false);
        vehicleTwoBox->setChecked(false);
        vehicleThreeBox->setChecked(false);
        vehicleFourBox->setChecked(false);
        vehicleFiveBox->setChecked(false);
        backButton->setEnabled(false);
        exitButton->setEnabled(false);
        switch (menuNumber) {
        case 0:
            vehicleButton->setEnabled(false);
            missionButton->setEnabled(false);

            break;
        case 1:
            roverButton->setEnabled(false);
            quadButton->setEnabled(false);
            fwuavButton->setEnabled(false);
            missionFromVehicleButton->setEnabled(false);
            break;
        case 2:

            break;
        default:
            break;
        }
    }
    isInEdit = !isInEdit;
}


void MapWindow::roverButtonClicked() {
    vehicleClicked = 1;
    setCursor(QCursor(roverImage));
}


void MapWindow::quadButtonClicked() {
    vehicleClicked = 2;
    setCursor(QCursor(quadImage));
}


void MapWindow::fwuavButtonClicked() {
    vehicleClicked = 3;
    setCursor(QCursor(fwuavImage));
}


void MapWindow::reachBoxClicked(int state) {
    if (state) {
        patrolBox->setChecked(false);
        noflyBox->setChecked(false);
        rectangleBox->setEnabled(false);
        circleBox->setEnabled(false);
        polygonBox->setEnabled(false);
        currentMission = Reach;
    }
    else {
        redraw();
        previousClicks.clear();
        currentMission = NoMission;
    }
}


void MapWindow::patrolBoxClicked(int state) {
    if (state) {
        reachBox->setChecked(false);
        noflyBox->setChecked(false);
        rectangleBox->setEnabled(true);
        circleBox->setEnabled(true);
        polygonBox->setEnabled(true);
        currentMission = Patrol;
    }
    else {
        redraw();
        previousClicks.clear();
        rectangleBox->setEnabled(false);
        circleBox->setEnabled(false);
        polygonBox->setEnabled(false);
        currentMission = NoMission;
    }
}


void MapWindow::noflyBoxClicked(int state) {
    if (state) {
        reachBox->setChecked(false);
        patrolBox->setChecked(false);
        rectangleBox->setEnabled(true);
        circleBox->setEnabled(true);
        polygonBox->setEnabled(true);
        currentMission = Nofly;
    }
    else {
        redraw();
        previousClicks.clear();
        rectangleBox->setEnabled(false);
        circleBox->setEnabled(false);
        polygonBox->setEnabled(false);
        currentMission = NoMission;
    }
}


void MapWindow::rectangleBoxClicked(int state) {
    if (state) {
        circleBox->setChecked(false);
        polygonBox->setChecked(false);
    }
    else {
        redraw();
        previousClicks.clear();
    }
}


void MapWindow::circleBoxClicked(int state) {
    if (state) {
        rectangleBox->setChecked(false);
        polygonBox->setChecked(false);
    }
    else {
        redraw();
        previousClicks.clear();
    }
}


void MapWindow::polygonBoxClicked(int state) {
    if (state) {
        circleBox->setChecked(false);
        rectangleBox->setChecked(false);
    }
    else {
        redraw();
        previousClicks.clear();
    }
}


void MapWindow::backButtonClicked() {
    isInHelp = false;
    switch (menuNumber) {
    case 1:
        vehicleButton->show();
        missionButton->show();
        roverButton->hide();
        quadButton->hide();
        fwuavButton->hide();
        missionFromVehicleButton->hide();
        backButton->hide();
        roverButtonHelp->hide();
        quadButtonHelp->hide();
        fwuavButtonHelp->hide();
        missionFromVehicleButtonHelp->hide();
        backButtonHelpVehicle->hide();
        menuBarHelp->hide();
        vehicleBoxesHelp->hide();
        exitButtonHelp->hide();
        menuNumber = 0;
        vehicleClicked = 0;
        setCursor(Qt::ArrowCursor);
        break;
    case 2:
        vehicleButton->show();
        missionButton->show();
        reachBox->hide();
        reachBox->setChecked(false);
        patrolBox->hide();
        patrolBox->setChecked(false);
        noflyBox->hide();
        noflyBox->setChecked(false);
        rectangleBox->hide();
        rectangleBox->setChecked(false);
        circleBox->hide();
        circleBox->setChecked(false);
        polygonBox->hide();
        polygonBox->setChecked(false);
        backButton->hide();
        reachBoxHelp->hide();
        patrolBoxHelp->hide();
        noflyBoxHelp->hide();
        rectangleBoxHelp->hide();
        circleBoxHelp->hide();
        polygonBoxHelp->hide();
        backButtonHelpMission->hide();
        menuBarHelp->hide();
        vehicleBoxesHelp->hide();
        exitButtonHelp->hide();
        menuNumber = 0;
        break;

    default:
        break;
    }
}


void MapWindow::vehicleOneBoxClicked(int state) {
    if (state) {
        vehicleTwoBox->setChecked(false);
        vehicleThreeBox->setChecked(false);
        vehicleFourBox->setChecked(false);
        vehicleFiveBox->setChecked(false);
        reachBox->setEnabled(true);
        patrolBox->setEnabled(true);
        noflyBox->setEnabled(true);
        selectedVehicle = vehicles[0];
        outline(vehicles[0]);
    }
    else {
        reachBox->setEnabled(false);
        patrolBox->setEnabled(false);
        noflyBox->setEnabled(false);
        reachBox->setChecked(false);
        patrolBox->setChecked(false);
        noflyBox->setChecked(false);
        selectedVehicle = nullptr;
        previousClicks.clear();
        redraw();
    }
}


void MapWindow::vehicleTwoBoxClicked(int state) {
    if (state) {
        vehicleOneBox->setChecked(false);
        vehicleThreeBox->setChecked(false);
        vehicleFourBox->setChecked(false);
        vehicleFiveBox->setChecked(false);
        reachBox->setEnabled(true);
        patrolBox->setEnabled(true);
        noflyBox->setEnabled(true);
        selectedVehicle = vehicles[1];
        outline(vehicles[1]);
    }
    else {
        reachBox->setEnabled(false);
        patrolBox->setEnabled(false);
        noflyBox->setEnabled(false);
        reachBox->setChecked(false);
        patrolBox->setChecked(false);
        noflyBox->setChecked(false);
        selectedVehicle = nullptr;
        previousClicks.clear();
        redraw();
    }
}


void MapWindow::vehicleThreeBoxClicked(int state) {
    if (state) {

        vehicleOneBox->setChecked(false);
        vehicleTwoBox->setChecked(false);
        vehicleFourBox->setChecked(false);
        vehicleFiveBox->setChecked(false);
        reachBox->setEnabled(true);
        patrolBox->setEnabled(true);
        noflyBox->setEnabled(true);
        selectedVehicle = vehicles[2];
        outline(vehicles[2]);
    }
    else {
        reachBox->setEnabled(false);
        patrolBox->setEnabled(false);
        noflyBox->setEnabled(false);
        reachBox->setChecked(false);
        patrolBox->setChecked(false);
        noflyBox->setChecked(false);
        selectedVehicle = nullptr;
        previousClicks.clear();
        redraw();
    }
}


void MapWindow::vehicleFourBoxClicked(int state) {
    if (state) {

        vehicleOneBox->setChecked(false);
        vehicleTwoBox->setChecked(false);
        vehicleThreeBox->setChecked(false);
        vehicleFiveBox->setChecked(false);
        reachBox->setEnabled(true);
        patrolBox->setEnabled(true);
        noflyBox->setEnabled(true);
        selectedVehicle = vehicles[3];
        outline(vehicles[3]);
    }
    else {
        reachBox->setEnabled(false);
        patrolBox->setEnabled(false);
        noflyBox->setEnabled(false);
        reachBox->setChecked(false);
        patrolBox->setChecked(false);
        noflyBox->setChecked(false);
        selectedVehicle = nullptr;
        previousClicks.clear();
        redraw();
    }
}


void MapWindow::vehicleFiveBoxClicked(int state) {
    if (state) {

        vehicleOneBox->setChecked(false);
        vehicleTwoBox->setChecked(false);
        vehicleThreeBox->setChecked(false);
        vehicleFourBox->setChecked(false);
        reachBox->setEnabled(true);
        patrolBox->setEnabled(true);
        noflyBox->setEnabled(true);
        selectedVehicle = vehicles[4];
        outline(vehicles[4]);
    }
    else {
        reachBox->setEnabled(false);
        patrolBox->setEnabled(false);
        noflyBox->setEnabled(false);
        reachBox->setChecked(false);
        patrolBox->setChecked(false);
        noflyBox->setChecked(false);
        selectedVehicle = nullptr;
        previousClicks.clear();
        redraw();
    }
}


void MapWindow::vehicleOneActDone() {
    if(!availableVehicles[0] && vehicleOneBox->isEnabled()) {
        vehicleOneBox->setChecked(true);
        vehicleOneBoxClicked(1);
    }
}


void MapWindow::vehicleTwoActDone() {
    if(!availableVehicles[1] && vehicleTwoBox->isEnabled()) {
        vehicleTwoBox->setChecked(true);
        vehicleTwoBoxClicked(1);
    }
}


void MapWindow::vehicleThreeActDone() {
    if(!availableVehicles[2] && vehicleThreeBox->isEnabled()) {
        vehicleThreeBox->setChecked(true);
        vehicleThreeBoxClicked(1);
    }
}


void MapWindow::vehicleFourActDone() {
    if(!availableVehicles[3] && vehicleFourBox->isEnabled()) {
        vehicleFourBox->setChecked(true);
        vehicleFourBoxClicked(1);
    }
}


void MapWindow::vehicleFiveActDone() {
    if(!availableVehicles[4] && vehicleFiveBox->isEnabled()) {
        vehicleFiveBox->setChecked(true);
        vehicleFiveBoxClicked(1);
    }
}


void MapWindow::deleteActDone(){
    if(toBeDeletedVehicle && vehicleOrShape)
        deleteVechicle();
    else if(toBeDeletedShape && !vehicleOrShape)
        deleteShape();
}


void MapWindow::helpActDone() {
    if(isInEdit) {
        if(!isInHelp)
            editHelp->show();
        else
            editHelp->hide();
    }
    else {
        if(!isInHelp) {
            menuBarHelp->show();
            vehicleBoxesHelp->show();
            exitButtonHelp->show();
        }
        else {
            menuBarHelp->hide();
            vehicleBoxesHelp->hide();
            exitButtonHelp->hide();
        }
        switch (menuNumber) {
        case 0: {
            if(!isInHelp) {
                vehicleButtonHelp->show();
                missionButtonHelp->show();
            }
            else {
                vehicleButtonHelp->hide();
                missionButtonHelp->hide();
            }
            break;
        }
        case 1: {
            if(!isInHelp) {
                roverButtonHelp->show();
                quadButtonHelp->show();
                fwuavButtonHelp->show();
                missionFromVehicleButtonHelp->show();
                backButtonHelpVehicle->show();
            }
            else {
                roverButtonHelp->hide();
                quadButtonHelp->hide();
                fwuavButtonHelp->hide();
                missionFromVehicleButtonHelp->hide();
                backButtonHelpVehicle->hide();
            }
            break;
        }
        case 2: {
            if(!isInHelp) {
                reachBoxHelp->show();
                patrolBoxHelp->show();
                noflyBoxHelp->show();
                rectangleBoxHelp->show();
                circleBoxHelp->show();
                polygonBoxHelp->show();
                backButtonHelpMission->show();
            }
            else {
                reachBoxHelp->hide();
                patrolBoxHelp->hide();
                noflyBoxHelp->hide();
                rectangleBoxHelp->hide();
                circleBoxHelp->hide();
                polygonBoxHelp->hide();
                backButtonHelpMission->hide();
            }
            break;
        }
        default:
            Q_ASSERT(false);
            break;
        }
    }
    isInHelp = !isInHelp;
}


void MapWindow::setupImages() {
    roverImage = QPixmap(":/Icons/rover.png").scaled(100,100,Qt::KeepAspectRatio);
    quadImage = QPixmap(":/Icons/quad.png").scaled(100,100,Qt::KeepAspectRatio);
    fwuavImage = QPixmap(":/Icons/fwuav.png").scaled(100,100,Qt::KeepAspectRatio);
}


void MapWindow::setupWidgets() {
    QSize screenSize = QGuiApplication::primaryScreen()->availableSize();
    //QSize screenSize = QGuiApplication::screens()[1]->availableSize();
    QSizeF physicalScreenSize = QGuiApplication::primaryScreen()->physicalSize();
    //QSizeF physicalScreenSize = QGuiApplication::screens()[1]->physicalSize();
    //double resolution = screenSize.width() / physicalScreenSize.width();

    const int BUTTON_WIDTH = int(screenSize.height() / 10.0);
    const int BUTTON_HEIGHT = int(screenSize.height() / 10.0);
    const int WINDOWS_TOOLBAR_HEIGHT = 45;
    const int SCROLL_BAR_THICKNESS = 50;
    const int MENU_BAR_OFFSET = 15;
    const int MENU_BAR_SIZE = menuBar()->height();
    const int BUTTON_OFFSET = 5;
    const int FONT_SIZE = int(physicalScreenSize.width() / 40.0);
    const int TEXT_LABEL_WIDTH = int(screenSize.width() / 4.0);
    const int TEXT_LABEL_HEIGHT = BUTTON_HEIGHT;
    const int HELP_FONT_SIZE = int(physicalScreenSize.width() / 32.0);

    mapDisplay = new ImageLabel(
                DisplayInfo(
                    QPoint(),
                    QSize(screenSize)),
                originalImage,
                this);
    mapDisplay->resize(zoomAmount * mapDisplay->pixmap()->size());
    mapDisplay->setScaledContents(true);

    mapScroll = new QScrollArea(this);
    mapScroll->move(0, MENU_BAR_SIZE + MENU_BAR_OFFSET);
    mapScroll->resize(screenSize.width(), screenSize.height() - WINDOWS_TOOLBAR_HEIGHT - MENU_BAR_SIZE - MENU_BAR_OFFSET);
    mapScroll->setBackgroundRole(QPalette::Dark);
    mapScroll->setWidget(mapDisplay);
    mapScroll->show();

    vehicleButton = new PushButton(
                DisplayInfo(
                    QPoint(screenSize.width() - BUTTON_WIDTH - SCROLL_BAR_THICKNESS, MENU_BAR_SIZE + MENU_BAR_OFFSET + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Vehicle\nSelection"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));

    missionButton = new PushButton(
                DisplayInfo(
                    QPoint(vehicleButton->pos().x(), vehicleButton->pos().y() + vehicleButton->size().height() + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Mission\nSelection"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));
    missionButton->setEnabled(false);

    exitButton = new PushButton(
                DisplayInfo(
                    QPoint(vehicleButton->pos().x(), screenSize.height() - WINDOWS_TOOLBAR_HEIGHT - BUTTON_HEIGHT - SCROLL_BAR_THICKNESS),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Exit to\nMain Menu"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));

    roverButton = new PushButton(
                DisplayInfo(
                    QPoint(screenSize.width() - BUTTON_WIDTH - SCROLL_BAR_THICKNESS, MENU_BAR_SIZE + MENU_BAR_OFFSET + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString(""),
                    QFont("Times", FONT_SIZE)),
                this,
                QString(":/Icons/rover.png"));
    roverButton->hide();

    quadButton = new PushButton(
                DisplayInfo(
                    QPoint(roverButton->pos().x(), roverButton->pos().y() + roverButton->size().height() + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString(""),
                    QFont("Times", FONT_SIZE)),
                this,
                QString(":/Icons/quad.png"));
    quadButton->hide();

    fwuavButton = new PushButton(
                DisplayInfo(
                    QPoint(roverButton->pos().x(), quadButton->pos().y() + quadButton->size().height() + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString(""),
                    QFont("Times", FONT_SIZE)),
                this,
                QString(":/Icons/fwuav.png"));
    fwuavButton->hide();

    missionFromVehicleButton = new PushButton(DisplayInfo(
                                                  QPoint(roverButton->pos().x(), fwuavButton->pos().y() + fwuavButton->size().height() + BUTTON_OFFSET),
                                                  QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                                              TextInfo(
                                                  QString("Mission\nSelection"),
                                                  QFont("Times", FONT_SIZE)),
                                              this,
                                              QString("a"));
    missionFromVehicleButton->hide();
    missionFromVehicleButton->setEnabled(false);

    reachBox = new CheckBox(
                DisplayInfo(
                    QPoint(screenSize.width() - BUTTON_WIDTH - SCROLL_BAR_THICKNESS, MENU_BAR_SIZE + MENU_BAR_OFFSET + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Reach"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));
    reachBox->hide();
    reachBox->setEnabled(false);

    patrolBox = new CheckBox(
                DisplayInfo(
                    QPoint(roverButton->pos().x(), roverButton->pos().y() + roverButton->size().height() + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Patrol"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));
    patrolBox->hide();
    patrolBox->setEnabled(false);

    noflyBox = new CheckBox(
                DisplayInfo(
                    QPoint(roverButton->pos().x(), quadButton->pos().y() + quadButton->size().height() + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Nofly"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));
    noflyBox->hide();
    noflyBox->setEnabled(false);

    rectangleBox = new CheckBox(
                DisplayInfo(
                    QPoint(noflyBox->pos().x(), noflyBox->pos().y() + noflyBox->size().height() + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString(""),
                    QFont("Times", FONT_SIZE)),
                this,
                QString(":/Icons/rectangle.png"));
    rectangleBox->hide();
    rectangleBox->setEnabled(false);

    circleBox = new CheckBox(
                DisplayInfo(
                    QPoint(rectangleBox->pos().x(), rectangleBox->pos().y() + rectangleBox->size().height() + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString(""),
                    QFont("Times", FONT_SIZE)),
                this,
                QString(":/Icons/circle.png"));
    circleBox->hide();
    circleBox->setEnabled(false);

    polygonBox = new CheckBox(
                DisplayInfo(
                    QPoint(circleBox->pos().x(), circleBox->pos().y() + circleBox->size().height() + BUTTON_OFFSET),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString(""),
                    QFont("Times", FONT_SIZE)),
                this,
                QString(":/Icons/polygon.png"));
    polygonBox->hide();
    polygonBox->setEnabled(false);

    vehicleOneBox = new CheckBox(
                DisplayInfo(
                    QPoint(MENU_BAR_OFFSET, exitButton->pos().y()),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Vehicle 1"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));
    vehicleOneBox->hide();

    vehicleTwoBox = new CheckBox(
                DisplayInfo(
                    QPoint(vehicleOneBox->pos().x() + vehicleOneBox->size().width() + BUTTON_OFFSET, vehicleOneBox->pos().y()),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Vehicle 2"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));
    vehicleTwoBox->hide();

    vehicleThreeBox = new CheckBox(
                DisplayInfo(
                    QPoint(vehicleTwoBox->pos().x() + vehicleTwoBox->size().width() + BUTTON_OFFSET, vehicleTwoBox->pos().y()),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Vehicle 3"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));
    vehicleThreeBox->hide();

    vehicleFourBox = new CheckBox(
                DisplayInfo(
                    QPoint(vehicleThreeBox->pos().x() + vehicleThreeBox->size().width() + BUTTON_OFFSET, vehicleThreeBox->pos().y()),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Vehicle 4"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));
    vehicleFourBox->hide();

    vehicleFiveBox = new CheckBox(
                DisplayInfo(
                    QPoint(vehicleFourBox->pos().x() + vehicleFourBox->size().width() + BUTTON_OFFSET, vehicleFourBox->pos().y()),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Vehicle 5"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));
    vehicleFiveBox->hide();

    backButton = new PushButton(
                DisplayInfo(
                    QPoint(),
                    QSize(BUTTON_WIDTH, BUTTON_HEIGHT)),
                TextInfo(
                    QString("Back"),
                    QFont("Times", FONT_SIZE)),
                this,
                QString("a"));
    backButton->hide();

    menuBarHelp = new TextLabel(
                DisplayInfo(
                    QPoint(0, vehicleButton->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("You can find all the actions and their\n"
                            "shortcuts in the menubar."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    menuBarHelp->hide();

    vehicleBoxesHelp = new TextLabel(
                DisplayInfo(
                    QPoint(vehicleOneBox->getDisplay().pos.x(), vehicleOneBox->getDisplay().pos.y() - TEXT_LABEL_HEIGHT - BUTTON_OFFSET),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("You can select which vehicle you would\n"
                            "like to assign missions to by clicking\n"
                            "one of the boxes below."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    vehicleBoxesHelp->hide();

    exitButtonHelp = new TextLabel(
                DisplayInfo(
                    QPoint(exitButton->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, exitButton->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this in order to return to the\n"
                            "welcome menu where you can create or\n"
                            "open another project."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    exitButtonHelp->hide();

    vehicleButtonHelp = new TextLabel(
                DisplayInfo(
                    QPoint(vehicleButton->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, vehicleButton->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to move into the vehicle\n"
                            "placement menu."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    vehicleButtonHelp->hide();

    missionButtonHelp = new TextLabel(
                DisplayInfo(
                    QPoint(missionButton->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, missionButton->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to move into the mission\n"
                            "configuration menu. (Not availible when\n"
                            "there are no vehicles)"),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    missionButtonHelp->hide();

    roverButtonHelp = new TextLabel(
                DisplayInfo(
                    QPoint(roverButton->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, roverButton->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to pick up a rover, left click on\n"
                            "the map to drop it. Right click to cancel."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    roverButtonHelp->hide();

    quadButtonHelp = new TextLabel(
                DisplayInfo(
                    QPoint(quadButton->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, quadButton->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to pick up a quad, left click on\n"
                            "the map to drop it. Right click to cancel."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    quadButtonHelp->hide();

    fwuavButtonHelp = new TextLabel(
                DisplayInfo(
                    QPoint(fwuavButton->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, fwuavButton->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to pick up a fwuav, left click on\n"
                            "the map to drop it. Right click to cancel."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    fwuavButtonHelp->hide();

    missionFromVehicleButtonHelp = new TextLabel(
                DisplayInfo(
                    QPoint(missionFromVehicleButton->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, missionFromVehicleButton->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to move into the mission\n"
                            "configuration menu. (Not availible when\n"
                            "there are no vehicles)"),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    missionFromVehicleButtonHelp->hide();

    backButtonHelpVehicle = new TextLabel(
                DisplayInfo(
                    QPoint(missionFromVehicleButton->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, missionFromVehicleButton->getDisplay().pos.y() + BUTTON_HEIGHT + BUTTON_OFFSET),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to go back to the main menu."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    backButtonHelpVehicle->hide();

    reachBoxHelp = new TextLabel(
                DisplayInfo(
                    QPoint(reachBox->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, reachBox->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to start configuring a reach\n"
                            "mission. Left click on the map to place\n"
                            "points. Right click to cancel, left click\n"
                            "on the first point to confirm mission."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    reachBoxHelp->hide();

    patrolBoxHelp = new TextLabel(
                DisplayInfo(
                    QPoint(patrolBox->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, patrolBox->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to start configuring a patrol\n"
                            "mission. Then select a shape from below."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    patrolBoxHelp->hide();

    noflyBoxHelp = new TextLabel(
                DisplayInfo(
                    QPoint(noflyBox->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, noflyBox->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to start configuring a nofly\n"
                            "mission. Then select a shape from below."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    noflyBoxHelp->hide();

    rectangleBoxHelp = new TextLabel(
                DisplayInfo(
                    QPoint(rectangleBox->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, rectangleBox->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to start configuring the mission\n"
                            "in a rectangular area. Left click two\n"
                            "opposite corners of the rectangle to draw\n"
                            "it. Right click to cancel."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    rectangleBoxHelp->hide();

    circleBoxHelp = new TextLabel(
                DisplayInfo(
                    QPoint(circleBox->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, circleBox->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to start configuring the mission\n"
                            "in a circular area. Left click the center\n"
                            "and then any point on the circle to draw\n"
                            "it. Right click to cancel."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    circleBoxHelp->hide();

    polygonBoxHelp = new TextLabel(
                DisplayInfo(
                    QPoint(polygonBox->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, polygonBox->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to start configuring the mission\n"
                            "in a polygon area. Left click for every\n"
                            "corner. Left click the first corner to\n"
                            "confirm. Right click to cancel."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    polygonBoxHelp->hide();

    backButtonHelpMission = new TextLabel(
                DisplayInfo(
                    QPoint(polygonBox->getDisplay().pos.x() - TEXT_LABEL_WIDTH - BUTTON_OFFSET, polygonBox->getDisplay().pos.y() + BUTTON_HEIGHT + BUTTON_OFFSET),
                    QSize(TEXT_LABEL_WIDTH, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("Press this to go back to the main menu."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    backButtonHelpMission->hide();

    editHelp = new TextLabel(
                DisplayInfo(
                    QPoint(vehicleButton->getDisplay().pos.x() - TEXT_LABEL_WIDTH * 2 - BUTTON_OFFSET, vehicleButton->getDisplay().pos.y()),
                    QSize(TEXT_LABEL_WIDTH * 2, TEXT_LABEL_HEIGHT)),
                TextInfo(
                    QString("You are currently in the edit menu. You can edit the vehicles' or the\n"
                            "missions' positions by left clicking to pick up and then left clicking\n"
                            "to place them. After picking up the vehicle or mission, you can right\n"
                            "click to cancel.You can press CTRL+E anytime to leave the edit menu."),
                    QFont("Times", HELP_FONT_SIZE)),
                this);
    editHelp->hide();
}


void MapWindow::setupActions() {
    saveAct = new QAction("Save");
    saveAct->setShortcut(QKeySequence::Save);

    saveAsAct = new QAction("Save As");
    saveAsAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);

    exitAct = new QAction("Exit");
    exitAct->setShortcut(Qt::Key_Escape);

    undoAct = new QAction("Undo");
    undoAct->setShortcut(QKeySequence::Undo);

    zoomInAct = new QAction("Zoom In");
    zoomInAct->setShortcut(QKeySequence::ZoomIn);

    zoomOutAct = new QAction("Zoom Out");
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);

    deleteAct = new QAction("Delete");
    deleteAct->setShortcut(Qt::Key_Delete);

    helpAct = new QAction("Help");
    helpAct->setShortcut(Qt::CTRL + Qt::Key_H);

    vehicleAct = new QAction("Vehicle Selection");
    vehicleAct->setShortcut(Qt::CTRL + Qt::Key_V);

    missionAct = new QAction("Mission Selection");
    missionAct->setShortcut(Qt::CTRL + Qt::Key_M);

    editAct = new QAction("Edit");
    editAct->setShortcut(Qt::CTRL + Qt::Key_E);

    backAct = new QAction("Back");
    backAct->setShortcut(Qt::CTRL + Qt::Key_B);

    vehicleOneAct = new QAction("Vehicle One");
    vehicleOneAct->setShortcut(Qt::CTRL + Qt::Key_1);

    vehicleTwoAct = new QAction("Vehicle Two");
    vehicleTwoAct->setShortcut(Qt::CTRL + Qt::Key_2);

    vehicleThreeAct = new QAction("Vehicle Three");
    vehicleThreeAct->setShortcut(Qt::CTRL + Qt::Key_3);

    vehicleFourAct = new QAction("Vehicle Four");
    vehicleFourAct->setShortcut(Qt::CTRL + Qt::Key_4);

    vehicleFiveAct = new QAction("Vehicle Five");
    vehicleFiveAct->setShortcut(Qt::CTRL + Qt::Key_5);
}


void MapWindow::setupMenus() {
    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exitAct);

    toolMenu = menuBar()->addMenu("Tools");
    toolMenu->addAction(undoAct);
    toolMenu->addAction(zoomInAct);
    toolMenu->addAction(zoomOutAct);
    toolMenu->addAction(deleteAct);
    toolMenu->addAction(helpAct);

    menuMenu = menuBar()->addMenu("Menus");
    menuMenu->addAction(vehicleAct);
    menuMenu->addAction(missionAct);
    menuMenu->addAction(editAct);
    menuMenu->addAction(backAct);

    vehicleMenu = menuBar()->addMenu("Vehicles");
    vehicleMenu->addAction(vehicleOneAct);
    vehicleMenu->addAction(vehicleTwoAct);
    vehicleMenu->addAction(vehicleThreeAct);
    vehicleMenu->addAction(vehicleFourAct);
    vehicleMenu->addAction(vehicleFiveAct);
}


void MapWindow::setupConnections() {
    connect(mapDisplay,SIGNAL(zoomIn(QPoint)),this,SLOT(zoomIn(QPoint)));
    connect(mapDisplay,SIGNAL(zoomOut(QPoint)),this,SLOT(zoomOut(QPoint)));
    connect(mapDisplay,SIGNAL(mouseLeftClick(QPoint)),this,SLOT(mouseLeftClick(QPoint)));
    connect(mapDisplay,SIGNAL(mouseRightClick(QPoint)),this,SLOT(mouseRightClick(QPoint)));

    connect(exitButton,SIGNAL(clicked()),this,SLOT(exitButtonClicked()));
    connect(vehicleButton,SIGNAL(clicked()),this,SLOT(vehicleButtonClicked()));
    connect(missionButton,SIGNAL(clicked()),this,SLOT(missionButtonClicked()));
    connect(backButton,SIGNAL(clicked()),this,SLOT(backButtonClicked()));

    connect(saveAct, &QAction::triggered, this, &MapWindow::saveActDone);
    connect(saveAsAct, &QAction::triggered, this, &MapWindow::saveAsActDone);
    connect(exitAct, &QAction::triggered, this, &MapWindow::exitButtonClicked);

    connect(undoAct, &QAction::triggered, this, &MapWindow::undoActDone);
    connect(zoomInAct, &QAction::triggered, this, &MapWindow::zoomInShortcut);
    connect(zoomOutAct, &QAction::triggered, this, &MapWindow::zoomOutShortcut);
    connect(deleteAct, &QAction::triggered, this, &MapWindow::deleteActDone);
    connect(helpAct, &QAction::triggered, this, &MapWindow::helpActDone);

    connect(vehicleAct, &QAction::triggered, this, &MapWindow::vehicleButtonClicked);
    connect(missionAct, &QAction::triggered, this, &MapWindow::missionButtonClicked);
    connect(editAct, &QAction::triggered, this, &MapWindow::editActDone);
    connect(backAct, &QAction::triggered, this, &MapWindow::backButtonClicked);

    connect(vehicleOneAct, &QAction::triggered, this, &MapWindow::vehicleOneActDone);
    connect(vehicleTwoAct, &QAction::triggered, this, &MapWindow::vehicleTwoActDone);
    connect(vehicleThreeAct, &QAction::triggered, this, &MapWindow::vehicleThreeActDone);
    connect(vehicleFourAct, &QAction::triggered, this, &MapWindow::vehicleFourActDone);
    connect(vehicleFiveAct, &QAction::triggered, this, &MapWindow::vehicleFiveActDone);

    connect(roverButton,SIGNAL(clicked()),this,SLOT(roverButtonClicked()));
    connect(quadButton,SIGNAL(clicked()),this,SLOT(quadButtonClicked()));
    connect(fwuavButton,SIGNAL(clicked()),this,SLOT(fwuavButtonClicked()));
    connect(missionFromVehicleButton,SIGNAL(clicked()),this,SLOT(missionButtonClicked()));

    connect(reachBox,SIGNAL(stateChanged(int)),this,SLOT(reachBoxClicked(int)));
    connect(patrolBox,SIGNAL(stateChanged(int)),this,SLOT(patrolBoxClicked(int)));
    connect(noflyBox,SIGNAL(stateChanged(int)),this,SLOT(noflyBoxClicked(int)));

    connect(rectangleBox,SIGNAL(stateChanged(int)),this,SLOT(rectangleBoxClicked(int)));
    connect(circleBox,SIGNAL(stateChanged(int)),this,SLOT(circleBoxClicked(int)));
    connect(polygonBox,SIGNAL(stateChanged(int)),this,SLOT(polygonBoxClicked(int)));

    connect(vehicleOneBox,SIGNAL(stateChanged(int)),this,SLOT(vehicleOneBoxClicked(int)));
    connect(vehicleTwoBox,SIGNAL(stateChanged(int)),this,SLOT(vehicleTwoBoxClicked(int)));
    connect(vehicleThreeBox,SIGNAL(stateChanged(int)),this,SLOT(vehicleThreeBoxClicked(int)));
    connect(vehicleFourBox,SIGNAL(stateChanged(int)),this,SLOT(vehicleFourBoxClicked(int)));
    connect(vehicleFiveBox,SIGNAL(stateChanged(int)),this,SLOT(vehicleFiveBoxClicked(int)));
}


void MapWindow::setupVehiclesAndMissionsFromFile(QString dataFilename) {
    QFile dataFile(dataFilename);
    dataFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&dataFile);
    QString trash, a0, a1, a2, a3, a4;
    stream >> trash >> trash >> trash >> trash >> pixelLength >> trash >> realLength >> trash >> pixelOrigin.rx() >> pixelOrigin.ry() >> trash >> vehicleAmount >> trash >> a0 >> a1 >> a2 >> a3 >> a4 >> trash;
    availableVehicles[0] = a0.toInt();
    availableVehicles[1] = a1.toInt();
    availableVehicles[2] = a2.toInt();
    availableVehicles[3] = a3.toInt();
    availableVehicles[4] = a4.toInt();
    if(vehicleAmount > 0) {
        missionButton->setEnabled(true);
        missionFromVehicleButton->setEnabled(true);
    }

    const int VEHICLE_SIZE = 100;
    int vehicleCount = 0;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if(line == "END_QMP_ENCODING")
            break;
        QTextStream lineStream(&line, QIODevice::ReadOnly | QIODevice::Text);
        QString identify;
        lineStream >> identify;
        if(identify == "Vehicle") {
            QString vehicleType;
            lineStream >> vehicleType;
            QString posX, posY;
            QVector<QString> parameters;
            lineStream >> posX >> posY;
            QString parametersWord;
            lineStream >> parametersWord;
            while(!lineStream.atEnd()) {
                QString parameter;
                lineStream >> parameter;
                if(parameter != "")
                    parameters.push_back(parameter);
            }
            QVector<double> parametersNum;
            for(int i = 0; i < parameters.size(); i ++)
                parametersNum.push_back(parameters[i].toDouble());

            int vehicleNumber = findRealIndex(vehicleCount);

            ///display the correct button and select the correct colors for the given free slot
            QColor borderColor;
            QColor fillColor;
            switch (vehicleNumber) {
            case 0:
                vehicleOneBox->show();
                borderColor = QColor(Qt::red);
                fillColor = QColor(Qt::red);
                break;
            case 1:
                vehicleTwoBox->show();
                borderColor = QColor(Qt::darkGreen);
                fillColor = QColor(Qt::darkGreen);
                break;
            case 2:
                vehicleThreeBox->show();
                borderColor = QColor(Qt::blue);
                fillColor = QColor(Qt::blue);
                break;
            case 3:
                vehicleFourBox->show();
                borderColor = QColor(Qt::magenta);
                fillColor = QColor(Qt::magenta);
                break;
            case 4:
                vehicleFiveBox->show();
                borderColor = QColor(Qt::yellow);
                fillColor = QColor(Qt::yellow);
                break;
            default:
                Q_ASSERT(false);
                break;
            }

            Vehicle *vehicle;
            switch (vehicleType.toInt()) {
            case 1:
                vehicle = new Rover(
                            DisplayInfo(
                                QPoint(posX.toInt() - VEHICLE_SIZE / 2, posY.toInt() - VEHICLE_SIZE / 2),
                                QSize(VEHICLE_SIZE, VEHICLE_SIZE)),
                            roverImage,
                            borderColor,
                            fillColor,
                            vehicleNumber,
                            typeRover);
                break;
            case 2:
                vehicle = new Quad(
                            DisplayInfo(
                                QPoint(posX.toInt() - VEHICLE_SIZE / 2, posY.toInt() - VEHICLE_SIZE / 2),
                                QSize(VEHICLE_SIZE, VEHICLE_SIZE)),
                            quadImage,
                            borderColor,
                            fillColor,
                            vehicleNumber,
                            typeQuad);
                break;
            case 3:
                vehicle = new Fwuav(
                            DisplayInfo(
                                QPoint(posX.toInt() - VEHICLE_SIZE / 2, posY.toInt() - VEHICLE_SIZE / 2),
                                QSize(VEHICLE_SIZE, VEHICLE_SIZE)),
                            fwuavImage,
                            borderColor,
                            fillColor,
                            vehicleNumber,
                            typeFwuav);
                break;
            default:
                Q_ASSERT(false);
                break;
            }
            vehicle->setParameters(parametersNum);
            vehicles.replace(vehicleNumber, vehicle);
            vehicleCount++;
        }
        else if(identify == "Shape") {
            QString vehicleNum, shapeType, missionTypeString;
            lineStream >> vehicleNum >> shapeType >> missionTypeString;
            missionType mission;
            switch (missionTypeString.toInt()) {
            case 0:
                mission = Reach;
                break;
            case 1:
                mission = Patrol;
                break;
            case 2:
                mission = Nofly;
                break;
            default:
                Q_ASSERT(false);
                break;
            }
            if(shapeType == "Circle") {
                QString posX, posY, radius;
                lineStream >> posX >> posY >> radius;
                Circle *shape = new Circle(QPoint(posX.toInt(), posY.toInt()), radius.toDouble(), mission, vehicles[vehicleNum.toInt()]->getBorderColor(), vehicles[vehicleNum.toInt()]->getFillColor());
                vehicles[vehicleNum.toInt()]->addShape(shape);
            }
            else if(shapeType == "Rectangle") {
                QString topLeftPosX, topLeftPosY, bottomRightPosX, bottomRightPosY;
                lineStream >> topLeftPosX >> topLeftPosY >> bottomRightPosX >> bottomRightPosY;
                Rectangle *shape = new Rectangle(QPoint(topLeftPosX.toInt(), topLeftPosY.toInt()), QPoint(bottomRightPosX.toInt(), bottomRightPosY.toInt()), mission, vehicles[vehicleNum.toInt()]->getBorderColor(), vehicles[vehicleNum.toInt()]->getFillColor());
                vehicles[vehicleNum.toInt()]->addShape(shape);
            }
            else if(shapeType == "Polygon") {
                QVector<QPointF> points;
                while(!lineStream.atEnd()) {
                    QString posX, posY;
                    lineStream >> posX >> posY;
                    if (posX != "" && posY != "")
                        points.push_back(QPoint(posX.toInt(),posY.toInt()));
                }
                Polygon *shape = new Polygon(points, mission, vehicles[vehicleNum.toInt()]->getBorderColor(), vehicles[vehicleNum.toInt()]->getFillColor());
                vehicles[vehicleNum.toInt()]->addShape(shape);
            }
            else if(shapeType == "Reach") {
                QString reachTypeString;
                lineStream >> reachTypeString;
                reachMissionType reachMission;
                switch (reachTypeString.toInt()) {
                case 0:
                    reachMission = InOrderGoOnce;
                    break;
                case 1:
                    reachMission = NoOrderGoOnce;
                    break;
                case 2:
                    reachMission = InOrderGoAlways;
                    break;
                case 3:
                    reachMission = NoOrderGoAlways;
                    break;
                default:
                    Q_ASSERT(false);
                    break;
                }
                QVector<QPointF> points;
                while(!lineStream.atEnd()) {
                    QString posX, posY;
                    lineStream >> posX >> posY;
                    if (posX != "" && posY != "")
                        points.push_back(QPoint(posX.toInt(),posY.toInt()));
                }
                ReachPoints *shape = new ReachPoints(points, mission, reachMission, vehicles[vehicleNum.toInt()]->getBorderColor(), vehicles[vehicleNum.toInt()]->getFillColor());
                vehicles[vehicleNum.toInt()]->addShape(shape);
            }
        }
    }
    dataFile.close();
}


void MapWindow::scaleImage(double factor, QPoint mousePos) {
    zoomAmount *= factor;
    mapDisplay->resize(zoomAmount * mapDisplay->pixmap()->size());

    int scrollerHorizontalPos = mapScroll->horizontalScrollBar()->value() + mapScroll->horizontalScrollBar()->width() / 2;
    int scrollerVerticalPos = mapScroll->verticalScrollBar()->value() + mapScroll->verticalScrollBar()->height() / 2;

    QPoint scrollerPos(scrollerHorizontalPos, scrollerVerticalPos);
    QPoint mouseDistFromScroller = mousePos - scrollerPos;

    adjustScrollBar(mapScroll->horizontalScrollBar(), factor , mouseDistFromScroller.x());
    adjustScrollBar(mapScroll->verticalScrollBar(), factor, mouseDistFromScroller.y());
}


void MapWindow::adjustScrollBar(QScrollBar *scrollBar, double factor, int mouseDistanceFromScrollBar) {
    scrollBar->setValue(int(scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)) + mouseDistanceFromScrollBar);
}


void MapWindow::vehiclePlacement(QPoint mousePos) {
    const int VEHICLE_SIZE = 100;

    if (vehicleAmount >= 5)
        return;

    ///calculates the amount of free slots for vehicles (out of 5)
    int freeSlots = 0;
    for(int i = 0; i < 5; i++) {
        if(availableVehicles[i])
            freeSlots++;
    }

    ///if there are 5 free slots, there are no vehicles
    bool allEmpty = false;
    if(freeSlots == 5)
        allEmpty = true;

    ///and if there are no vehicles, missionbutton should become enabled (since this will be the first vehicle being placed
    if(allEmpty) {
        missionButton->setEnabled(true);
        missionFromVehicleButton->setEnabled(true);
    }

    int vehicleNumber;
    if(ongoingUndo) {
        vehicleNumber = vehicleStack.top()->getVehicleNumber();
    }

    else {
        ///find the first free slot and set it not free anymore
        int firstAvailableVehicle = 0;
        for(; firstAvailableVehicle < 5; firstAvailableVehicle++) {
            if(availableVehicles[firstAvailableVehicle])
                break;
        }
        vehicleNumber = firstAvailableVehicle;
    }

    availableVehicles[vehicleNumber] = false;

    ///display the correct button and select the correct colors for the given free slot
    QColor borderColor;
    QColor fillColor;
    switch (vehicleNumber) {
    case 0:
        vehicleOneBox->show();
        borderColor = QColor(Qt::red);
        fillColor = QColor(Qt::red);
        break;
    case 1:
        vehicleTwoBox->show();
        borderColor = QColor(Qt::darkGreen);
        fillColor = QColor(Qt::darkGreen);
        break;
    case 2:
        vehicleThreeBox->show();
        borderColor = QColor(Qt::blue);
        fillColor = QColor(Qt::blue);
        break;
    case 3:
        vehicleFourBox->show();
        borderColor = QColor(Qt::magenta);
        fillColor = QColor(Qt::magenta);
        break;
    case 4:
        vehicleFiveBox->show();
        borderColor = QColor(Qt::yellow);
        fillColor = QColor(Qt::yellow);
        break;
    default:
        break;
    }

    ///increase the amount of vehicles by one
    vehicleAmount++;

    int vehicleType;
    if(ongoingUndo) {
        vehicleType = vehicleStack.top()->getVehicleType();
    }
    else {
        vehicleType = vehicleClicked;
    }
    Vehicle *vehicle;
    if(ongoingUndo)
        vehicle = vehicleStack.pop();
    else {
        switch (vehicleType) {
        case 1: {

            vehicle = new Rover(
                                DisplayInfo(
                                    QPoint(int(mousePos.x() / zoomAmount - VEHICLE_SIZE / 2), int(mousePos.y() / zoomAmount - VEHICLE_SIZE / 2)),
                                    QSize(int(VEHICLE_SIZE), int(VEHICLE_SIZE))),
                                roverImage,
                                borderColor,
                                fillColor,
                                vehicleNumber,
                                typeRover);
            VehicleInputDialog inputDialog("rover", {"Mass", "Speed"}, this);
            int res = inputDialog.exec();
            if(res) {
                QVector<double> inputs = inputDialog.getInputs();
                vehicle->setParameters(inputs);
            }
            else {
                vehicle->setParameters();
            }
            break;
        }
        case 2: {
            vehicle = new Quad(
                        DisplayInfo(
                            QPoint(int(mousePos.x() / zoomAmount - VEHICLE_SIZE / 2), int(mousePos.y() / zoomAmount - VEHICLE_SIZE / 2)),
                            QSize(int(VEHICLE_SIZE), int(VEHICLE_SIZE))),
                        quadImage,
                        borderColor,
                        fillColor,
                        vehicleNumber,
                        typeQuad);
            VehicleInputDialog inputDialog("quad", {"Mass", "Speed"}, this);
            int res = inputDialog.exec();
            if(res) {
                QVector<double> inputs = inputDialog.getInputs();
                vehicle->setParameters(inputs);
            }
            else {
                vehicle->setParameters();
            }
            break;
        }
        case 3: {
            vehicle = new Fwuav(
                        DisplayInfo(
                            QPoint(int(mousePos.x() / zoomAmount - VEHICLE_SIZE / 2), int(mousePos.y() / zoomAmount - VEHICLE_SIZE / 2)),
                            QSize(int(VEHICLE_SIZE), int(VEHICLE_SIZE))),
                        fwuavImage,
                        borderColor,
                        fillColor,
                        vehicleNumber,
                        typeFwuav);
            VehicleInputDialog inputDialog("fwuav", {"Mass", "Speed"}, this);
            int res = inputDialog.exec();
            if(res) {
                QVector<double> inputs = inputDialog.getInputs();
                vehicle->setParameters(inputs);
            }
            else {
                vehicle->setParameters();
            }
            break;
        }
        default:
            break;
        }
    }
    vehicles.replace(vehicleNumber, vehicle);
    if(!ongoingUndo) {
        operationStack.push(VehiclePlaced);
        vehicleStack.push(vehicle);
    }
    setCursor(Qt::ArrowCursor);
    vehicleClicked = 0;
    redraw();
}


void MapWindow::missionConfiguration(QPoint mousePos) {
    if(ongoingUndo) {
        Shape *shape = shapeStack.pop();
        vehicles[shape->getVehicleNumber()]->addShape(shape);
        redraw();
        return;
    }
    if(currentMission == Reach) {
        if(previousClicks.size() == 0 || QLineF(previousClicks[0], mousePos / zoomAmount).length() > 10) {
            previousClicks.push_back(mousePos / zoomAmount);
            drawMarker(mousePos);
        }
        else {
            bool isOkPressed;
            QInputDialog inputDialog;
            QString answer = inputDialog.getItem(this, "Mission Type", "Please select your reach mission type.",
                                                 QStringList({"In Order, Go Once", "No Order, Go Once", "In Order, Go Always", "No Order, Go Always"}),
                                                 0, true, &isOkPressed, Qt::MSWindowsFixedSizeDialogHint);
            if(!isOkPressed)
                return;

            reachMissionType reachType;
            if(answer == "In Order, Go Once")
                reachType = InOrderGoOnce;
            else if(answer == "No Order, Go Once")
                reachType = NoOrderGoOnce;
            else if(answer == "In Order, Go Always")
                reachType = InOrderGoAlways;
            else if(answer == "No Order, Go Always")
                reachType = NoOrderGoAlways;
            else
                Q_ASSERT(false);

            ReachPoints *shape = new ReachPoints(previousClicks, Reach, reachType, selectedVehicle->getBorderColor(), selectedVehicle->getFillColor());
            selectedVehicle->addShape(shape);
            if(!ongoingUndo) {
                operationStack.push(ShapePlaced);
                shapeStack.push(shape);
            }
            previousClicks.clear();
            redraw();
        }
    }
    else if(rectangleBox->isChecked()) {
        if(previousClicks.size() == 0) {
            previousClicks.push_back(mousePos / zoomAmount);
            drawMarker(mousePos);
        }
        else {
            Rectangle *shape = new Rectangle(previousClicks[0], mousePos / zoomAmount, currentMission, selectedVehicle->getBorderColor(), selectedVehicle->getFillColor());
            selectedVehicle->addShape(shape);
            if(!ongoingUndo) {
                operationStack.push(ShapePlaced);
                shapeStack.push(shape);
            }
            previousClicks.clear();
            redraw();
        }
    }
    else if(circleBox->isChecked()) {
        if(previousClicks.size() == 0) {
            previousClicks.push_back(mousePos / zoomAmount);
            drawMarker(mousePos);
        }
        else {
            QLineF line(previousClicks[0], mousePos / zoomAmount);
            Circle *shape = new Circle(previousClicks[0], line.length(), currentMission, selectedVehicle->getBorderColor(), selectedVehicle->getFillColor());
            selectedVehicle->addShape(shape);
            if(!ongoingUndo) {
                operationStack.push(ShapePlaced);
                shapeStack.push(shape);
            }
            previousClicks.clear();
            redraw();
        }
    }
    else if(polygonBox->isChecked()) {
        if(previousClicks.size() < 3 || QLineF(previousClicks[0], mousePos / zoomAmount).length() > 10){
            previousClicks.push_back(mousePos / zoomAmount);
            drawMarker(mousePos);
        }
        else {
            Polygon *shape = new Polygon(previousClicks, currentMission, selectedVehicle->getBorderColor(), selectedVehicle->getFillColor());
            selectedVehicle->addShape(shape);
            if(!ongoingUndo) {
                operationStack.push(ShapePlaced);
                shapeStack.push(shape);
            }
            previousClicks.clear();
            redraw();
        }
    }
}


///this is a mess
void MapWindow::whichObjectIsPressed(QPoint mousePos) {
    redraw();
    for(int i = 0; i < vehicleAmount; i++) {
        int vehicleIndex = (i + nextVehicleIndex) % vehicleAmount;
        if(vehicles[findRealIndex(vehicleIndex)]->isInside(mousePos, zoomAmount)) {
            toBeDeletedVehicle = vehicles[findRealIndex(vehicleIndex)];
            nextVehicleIndex = vehicleIndex + 1;
            vehicleOrShape = true;
            highlight();
            return;
        }

        QVector<Shape*>* shapes = vehicles[findRealIndex(vehicleIndex)]->getShapes();
        for(int j = 0; j < shapes->size(); j++) {
            int shapeIndex = (j + nextShapeIndex) % shapes->size();
            if(shapes->at(shapeIndex)->isInside(mousePos, zoomAmount)){
                toBeDeletedShape = shapes->at(shapeIndex);
                nextVehicleIndex = vehicleIndex + 1;
                nextShapeIndex = shapeIndex + 1;
                vehicleOrShape = false;
                highlight();
                return;
            }
        }
    }
}


void MapWindow::highlight() {
    QPixmap pix = *(mapDisplay->pixmap());
    QImage img = pix.toImage();
    if(toBeDeletedVehicle)
        toBeDeletedVehicle->highlight(&img, 10);
    else if(toBeDeletedShape)
        toBeDeletedShape->highlight(&img, 10);
    mapDisplay->setPixmap(QPixmap::fromImage(img));
}


void MapWindow::outline(Vehicle *selected) {
    QPixmap pix = *(mapDisplay->pixmap());
    QImage img = pix.toImage();
    selected->highlight(&img, 5);
    mapDisplay->setPixmap(QPixmap::fromImage(img));
}


void MapWindow::deleteVechicle() {
    ///find the correct vehicle index
    int vehicleIndex = (nextVehicleIndex + vehicleAmount - 1) % vehicleAmount;

    ///if this is not an undo operation, prepare for a possible undo operation
    if(!ongoingUndo) {
        operationStack.push(VehicleRemoved);
        vehicleStack.push(vehicles[findRealIndex(vehicleIndex)]);
    }

    ///delete the selected vehicle
    vehicles[findRealIndex(vehicleIndex)] = new Vehicle();

    ///reset the pointers
    toBeDeletedVehicle = nullptr;
    toBeDeletedShape = nullptr;

    ///reset the indexes
    nextVehicleIndex = 0;
    nextShapeIndex = 0;

    ///hide the appropriate button
    switch (findRealIndex(vehicleIndex)) {
    case 0:
        vehicleOneBox->hide();
        break;
    case 1:
        vehicleTwoBox->hide();
        break;
    case 2:
        vehicleThreeBox->hide();
        break;
    case 3:
        vehicleFourBox->hide();
        break;
    case 4:
        vehicleFiveBox->hide();
        break;
    default:
        break;
    }

    ///decrease the vehicle amount
    vehicleAmount--;

    ///update the boolean array
    availableVehicles[findRealIndex(vehicleIndex)] = true;

    ///if there are no vehicles left, disable the mission button and exit the mission menu if the user is in there
    if(vehicleAmount == 0) {
        missionButton->setEnabled(false);
        missionFromVehicleButton->setEnabled(false);
        if(menuNumber == 2)
            backButtonClicked();
    }
    redraw();
}


void MapWindow::deleteShape() {
    int vehicleIndex = (nextVehicleIndex + vehicleAmount - 1) % vehicleAmount;
    QVector<Shape*>* shapes = vehicles[findRealIndex(vehicleIndex)]->getShapes();

    if(!ongoingUndo){
        operationStack.push(ShapeRemoved);
        shapeStack.push(shapes->at((nextShapeIndex - 1) % shapes->size()));
    }

    shapes->removeAt((nextShapeIndex - 1) % shapes->size());

    toBeDeletedVehicle = nullptr;
    toBeDeletedShape = nullptr;
    nextVehicleIndex = 0;
    nextShapeIndex = 0;
    redraw();
}


void MapWindow::clickAndMove(QPoint mousePos) {
    if(previousClicks.size() == 0) {
        for(int i = 0; i < vehicleAmount; i++) {
            if(vehicles[findRealIndex(i)]->isInside(mousePos, zoomAmount)) {
                toBeEditedVehicle = vehicles[findRealIndex(i)];
                break;
            }
            QVector<Shape*>* shapes = vehicles[findRealIndex(i)]->getShapes();
            for(int j = 0; j < shapes->size(); j++) {
                if(shapes->at(j)->isInside(mousePos, zoomAmount)){
                    toBeEditedShape = shapes->at(j);
                    break;
                }
            }
            if(toBeEditedShape)
                break;
        }
        if(toBeEditedVehicle) {
            redrawWithoutVehicle(toBeEditedVehicle);
            previousClicks.push_back(mousePos / zoomAmount);
            setCursor(QCursor(toBeEditedVehicle->getPixmap()));
        }
        else if(toBeEditedShape) {
            redrawWithoutShape(toBeEditedShape);
            previousClicks.push_back(mousePos / zoomAmount);
            setCursor(Qt::BusyCursor);
        }
        else {
            return;
        }
    }
    else {
        setCursor(Qt::ArrowCursor);
        if(toBeEditedVehicle)
            toBeEditedVehicle->move(QPoint(int(mousePos.x() / zoomAmount) - int(previousClicks[0].x()), int(mousePos.y() / zoomAmount) - int(previousClicks[0].y())));
        else if(toBeEditedShape) {
            toBeEditedShape->move(QPoint(int(mousePos.x() / zoomAmount) - int(previousClicks[0].x()), int(mousePos.y() / zoomAmount) - int(previousClicks[0].y())));
        }
        previousClicks.clear();
        toBeEditedVehicle = nullptr;
        toBeEditedShape = nullptr;
        redraw();
    }
}


int MapWindow::findRealIndex(int index) {
    int count = 0;
    for(int i = 0; i < 5; i++){
        if(!availableVehicles[i]) {
            if(index == count)
                return i;
            count++;
        }
    }
    ///you shouldn't be able to reach here
    return -1;
}


void MapWindow::drawMarker(QPoint mousePos) {
    const double PEN_WIDTH = 5;
    const double CIRCLE_RADIUS = 5;

    QPixmap pix = *(mapDisplay->pixmap());
    QImage img = pix.toImage();
    QPainter painter(&img);
    QPen pen(selectedVehicle->getBorderColor());
    pen.setWidth(int(PEN_WIDTH / zoomAmount));
    painter.setPen(pen);
    painter.drawEllipse(QPointF(mousePos) / zoomAmount, CIRCLE_RADIUS / zoomAmount, CIRCLE_RADIUS / zoomAmount);
    ///if it is the first click
    if(previousClicks.size() == 1)
        painter.drawEllipse(QPointF(mousePos) / zoomAmount, CIRCLE_RADIUS / (2 * zoomAmount), CIRCLE_RADIUS / (2 * zoomAmount));

    mapDisplay->setPixmap(QPixmap::fromImage(img));
}


void MapWindow::redraw() {
    QImage editedImage = originalImage;
    for(int i = 0; i < vehicleAmount; i++)
        vehicles[findRealIndex(i)]->draw(&editedImage);
    mapDisplay->setPixmap(QPixmap::fromImage(editedImage));
}


void MapWindow::redrawWithoutVehicle(Vehicle *vehicle) {
    QImage editedImage = originalImage;
    for(int i = 0; i < vehicleAmount; i++) {
        if(vehicles[findRealIndex((i))] == vehicle)
            vehicles[findRealIndex(i)]->drawWithoutVehicle(&editedImage);
        else
            vehicles[findRealIndex(i)]->draw(&editedImage);
    }
    mapDisplay->setPixmap(QPixmap::fromImage(editedImage));
}


void MapWindow::redrawWithoutShape(Shape *shape) {
    QImage editedImage = originalImage;
    for(int i = 0; i < vehicleAmount; i++)
        vehicles[findRealIndex(i)]->drawWithoutShape(&editedImage, shape);
    mapDisplay->setPixmap(QPixmap::fromImage(editedImage));
}
