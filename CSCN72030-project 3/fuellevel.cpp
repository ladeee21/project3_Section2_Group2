#include "stdafx.h"
#include "fuellevel.h"

using namespace std;

FuelLevel::FuelLevel(QWidget* parent)
    : QWidget(parent)
{
    // initialize variables to empty
    this->fuelPercentage = 0;
    this->currentFuelLevel = 0;
    this->lowFuelAlert = false;
    this->criticalFuelAlert = false;

    // start ui
    ui.setupUi(this);
}

FuelLevel::~FuelLevel()
{}

void FuelLevel::updateFuelLevel(float fuelLevel)
{
    // set fuel level and percentage
    this->currentFuelLevel = fuelLevel;
    setFuelPercentage(fuelLevel);

    // update fuel bar and label to fuel percentage
    ui.fuelBar->setValue(this->fuelPercentage);
    updateFuelPercentageLabel();

    // if fuel is critical turn yellow light on and red light off
    if (isFuelCritical())
    {
        ui.yellowLight->setVisible(false);
        ui.redLight->setVisible(true);

    }
    // if fuel is low turn red light on and yellow light off
    else if (isFuelLow())
    {
        ui.redLight->setVisible(false);
        ui.yellowLight->setVisible(true);
    }
    // turn both lights off
    else
    {
        ui.redLight->setVisible(false);
        ui.yellowLight->setVisible(false);
    }
}

void FuelLevel::updateFuelPercentageLabel()
{
    // set percent label to current fuel percentage
    QString percentageText = QString::number(this->fuelPercentage) + "%";
    ui.fuelPercent->setText(percentageText);
}

void FuelReader::readFuelData() {

    // open file in read mode
    QFile file(":/data/fuelLevel.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file";
        return;
    }

    // read one line at a time 
    QTextStream in(&file);
    while (1)
    {
        while (!in.atEnd()) {
            QString line = in.readLine();
            // lineRead is connected to updateFuelData so moves input data to main thread
            emit lineRead(line.toFloat());

            // make thread wait 
            QThread::msleep(500);
        }

        // move file pointer back to start
        in.seek(0);
    }
}
