/**
    MissionPlanner
    vehicleinputdialog.cpp
    Purpose: VehicleInputDialog class implementation

    @author Edin Guso
    @version 1.0
    @date 31/07/2019
*/


#include "vehicleinputdialog.h"

VehicleInputDialog::VehicleInputDialog(QString vehicle, QVector<QString> paramNames, QWidget *parent) : QDialog (parent, Qt::MSWindowsFixedSizeDialogHint) {
    QFormLayout *lytMain = new QFormLayout(this);

    QLabel *titleLabel = new QLabel(QString("Please enter the parameters for the %1:").arg(vehicle), this);
    lytMain->addWidget(titleLabel);

        for (int i = 0; i < paramNames.size(); ++i)
        {
            QLabel *tLabel = new QLabel(QString("%1:").arg(paramNames[i]), this);
            QLineEdit *tLine = new QLineEdit(this);
            tLine->setValidator(new QDoubleValidator(0, 10000, 2, this));
            lytMain->addRow(tLabel, tLine);
            fields << tLine;
        }

        QDialogButtonBox *buttonBox = new QDialogButtonBox
                ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                  Qt::Horizontal, this );
        buttonBox->button(QDialogButtonBox::Ok)->setText("Set Parameters");
        buttonBox->button(QDialogButtonBox::Cancel)->setText("Default Parameters");
        lytMain->addWidget(buttonBox);

        bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                            this, &VehicleInputDialog::checkInputs);
        Q_ASSERT(conn);
        conn = connect(buttonBox, &QDialogButtonBox::rejected,
                       this, &VehicleInputDialog::reject);
        Q_ASSERT(conn);

        setLayout(lytMain);

}


QVector<double> VehicleInputDialog::getInputs() {
    QVector<double> inputs;
    for(int i = 0; i < fields.size(); i++)
        inputs.push_back(fields[i]->text().toDouble());
    return inputs;
}


void VehicleInputDialog::checkInputs() {
    for(int i = 0; i < fields.size(); i++)
        if(fields[i]->text() == "")
            return;

    accept();
}
