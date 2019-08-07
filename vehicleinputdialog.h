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


class VehicleInputDialog : public QDialog
{
    Q_OBJECT
public:
    VehicleInputDialog(QString vehicle, QVector<QString> paramNames, QWidget *parent = nullptr);
    QVector<double> getInputs();

private slots:
    void checkInputs();

private:
    QList<QLineEdit*> fields;
};

#endif // VEHICLEINPUTDIALOG_H

