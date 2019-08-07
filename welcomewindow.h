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
*/
class WelcomeWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();

private slots:
    void openProjectClicked();
    void newProjectClicked();
    void scaleInputsAccepted();


private:
    void setupWidgets();
    void setupActions();
    void setupMenus();
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
