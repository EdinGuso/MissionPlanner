/**
    MissionPlanner
    vehicleinputdialog.h
    Purpose: VehicleInputDialog class declaration

    @author Edin Guso
    @version 1.0
    @date 31/07/2019
*/


#ifndef VEHICLEINPUTDIALOG_H
#define VEHICLEINPUTDIALOG_H


#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QPushButton>


/**
    @class VehicleInputDialog
    @inherits QDialog
    handles the vehicle parameter popup window

    @variable<fields> holds all the line edits to later access them and get inputs
*/
class VehicleInputDialog : public QDialog
{
    Q_OBJECT
public:
    /**
        Constructor

        @param<vehicle> used to display the type of the vehicle on the popup window
        @param<paramNames> a line edit for every member of paramNames will be created will be created
        @param<parent> pointer to the parent of the object, passed onto the @class<QDialog> constructor
    */
    VehicleInputDialog(QString vehicle, QVector<QString> paramNames, QWidget *parent = nullptr);


    /**
        @returns a vector containing all the information from line edits
    */
    QVector<double> getInputs();


private slots:
    /**
        Checks whether all the line edits have been filled if the user has pressed set parameters
    */
    void checkInputs();


private:
    QList<QLineEdit*> fields;
};


#endif // VEHICLEINPUTDIALOG_H

