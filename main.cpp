/**
    MissionPlanner
    main.cpp
    Purpose: Handles the program setup and constructs the welcome window.
    Also executes the main event loop until the program ends

    @author Edin Guso
    @version 1.0
    @date 16/07/2019
*/


#include "welcomewindow.h"
#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ///set taskbar icon
    a.setWindowIcon(QIcon(":/Icons/taskbar.png"));
    WelcomeWindow w;
    w.show();

    return a.exec();
}
