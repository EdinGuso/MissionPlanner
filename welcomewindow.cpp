/**
    MissionPlanner
    welcomewindow.cpp
    Purpose: WelcomeWindow class implementation

    @author Edin Guso
    @version 1.0
    @date 16/07/2019
*/


#include "welcomewindow.h"
#include "ui_welcomewindow.h"


WelcomeWindow::WelcomeWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::WelcomeWindow) {
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setupWidgets();
    setupActions();
    setupMenus();
    setupConnections();
}


WelcomeWindow::~WelcomeWindow() {
    delete ui;
    delete titleText;
    delete openProjectButton;
    delete newProjectButton;
    delete descriptionText;
    delete newProjectAct;
    delete openProjectAct;
    delete fileMenu;
    ///delete nextWindow; cannot delete this because at this point it is already deleted
}


void WelcomeWindow::openProjectClicked() {
    QString message = "Open a folder that contains a qmp configuration";
    QString location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString directoryName = QFileDialog::getExistingDirectory(this, message, location,
                                                              QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(directoryName.isNull() || directoryName.isEmpty()) {
        QMessageBox errorMessageBox(QMessageBox::Information, "Canceled",
                                    "Folder selection has been canceled.",
                                    QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
        errorMessageBox.exec();
        return;
    }

    QString imageFilename = directoryName + "/original_image.vmp";
    QString dataFilename = directoryName + "/data.qmp";
    QFile imageFile(imageFilename);
    if (!imageFile.open(QIODevice::ReadOnly)) {
        QMessageBox errorMessageBox(QMessageBox::Information, "File error",
                                    "The directory you entered does not contain the correct file types.",
                                    QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
        errorMessageBox.exec();
        return;
    }

    imageFile.close();
    QFile dataFile(dataFilename);
    if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox errorMessageBox(QMessageBox::Information, "File error",
                                    "The directory you entered does not contain the correct file types.",
                                    QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
        errorMessageBox.exec();
        return;
    }

    dataFile.close();

    nextWindow = new MapWindow(directoryName, this);
    nextWindow->show();
    this->hide();

}


void WelcomeWindow::newProjectClicked() {
    QString message = "Open a file containing a map";
    QString location = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filter = "JPEG Files (*.JPG) ;; PNG files (*.PNG)";
    QString filename = QFileDialog::getOpenFileName(this, message, location, filter);

    if(filename.isNull() || filename.isEmpty()) {
        QMessageBox errorMessageBox(QMessageBox::Information, "Canceled",
                                    "File selection has been canceled.",
                                    QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
        errorMessageBox.exec();
        return;
    }

    ///if the filename is invalid, return
    if(QPixmap(filename).isNull()) {
        QMessageBox errorMessageBox(QMessageBox::Information, "File error",
                                    "The file you entered is not of valid type.",
                                    QMessageBox::Ok, this, Qt::MSWindowsFixedSizeDialogHint);
        errorMessageBox.exec();
        return;
    }

    nextWindow = new MapWindow(QPixmap(filename).toImage(), this);

    ImageLabel *nextWindowMapDisplay = nextWindow->getMapDisplay();
    QSize displaySize = nextWindowMapDisplay->size();

    ScaleDialog dialog(QPixmap(filename).toImage(), pixelLength, realLength, pixelOrigin,
                       displaySize, QGuiApplication::primaryScreen()->availableSize());
    connect(&dialog, &ScaleDialog::accepted, this, &WelcomeWindow::scaleInputsAccepted);
    dialog.exec();
}


void WelcomeWindow::scaleInputsAccepted() {
    nextWindow->setPixelLength(pixelLength);
    nextWindow->setRealLength(realLength);
    nextWindow->setPixelOrigin(pixelOrigin);
    nextWindow->show();
    this->hide();
}


void WelcomeWindow::setupWidgets(){
    QSize screenSize = QGuiApplication::primaryScreen()->availableSize();
    QSizeF physicalScreenSize = QGuiApplication::primaryScreen()->physicalSize();

    const int WIDTH_CONSTANT = int(screenSize.width() / 6.0);
    const int HEIGHT_CONSTANT = int(screenSize.height() / 15.0);
    const int FONT_CONTSTANT = int(physicalScreenSize.height() / 10.0);

    titleText = new TextLabel(
                DisplayInfo(
                    QPoint(screenSize.width() / 2 - WIDTH_CONSTANT / 2, HEIGHT_CONSTANT * 3),
                    QSize(WIDTH_CONSTANT, HEIGHT_CONSTANT * 3)),
                TextInfo(
                    QString("Welcome to\nMissioon Planner\nsoftware!"),
                    QFont("Times", FONT_CONTSTANT)),
                this);

    openProjectButton = new PushButton(
                DisplayInfo(
                    QPoint(titleText->pos().x(), titleText->pos().y() + titleText->size().height()),
                    QSize(WIDTH_CONSTANT / 2, HEIGHT_CONSTANT)),
                TextInfo(
                    QString("Open Project"),
                    QFont("Times", FONT_CONTSTANT / 2)),
                this);

    newProjectButton = new PushButton(
                DisplayInfo(
                    QPoint(openProjectButton->pos().x() + openProjectButton->size().width(),
                           titleText->pos().y() + titleText->size().height()),
                    QSize(WIDTH_CONSTANT / 2, HEIGHT_CONSTANT)),
                TextInfo(
                    QString("New Project"),
                    QFont("Times", FONT_CONTSTANT / 2)),
                this);

    descriptionText = new TextLabel(
                DisplayInfo(
                    QPoint(openProjectButton->pos().x(),
                           openProjectButton->pos().y() + openProjectButton->size().height()),
                    QSize(WIDTH_CONSTANT, HEIGHT_CONSTANT * 3)),
                TextInfo(
                    QString("You should either load a previous\n"
                            "project or load a new map. After\n"
                            "that, a new screen will pop up where\n"
                            "you will be able to deploy vehicles\n"
                            "and setup missions. In the map\n"
                            "window, you can press CTRL+H at\n"
                            "any time to display the tutorial."),
                    QFont("Times", FONT_CONTSTANT / 2)),
                this);
}


void WelcomeWindow::setupActions(){
    newProjectAct = new QAction("New Project");
    newProjectAct->setShortcuts(QKeySequence::New);

    openProjectAct = new QAction("Open Project");
    openProjectAct->setShortcuts(QKeySequence::Open);
}


void WelcomeWindow::setupMenus(){
    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(newProjectAct);
    fileMenu->addAction(openProjectAct);
}


void WelcomeWindow::setupConnections(){
    connect(openProjectButton,SIGNAL(clicked()),this,SLOT(openProjectClicked()));
    connect(newProjectButton,SIGNAL(clicked()),this,SLOT(newProjectClicked()));
    connect(newProjectAct, &QAction::triggered, this, &WelcomeWindow::newProjectClicked);
    connect(openProjectAct, &QAction::triggered, this, &WelcomeWindow::openProjectClicked);
}
