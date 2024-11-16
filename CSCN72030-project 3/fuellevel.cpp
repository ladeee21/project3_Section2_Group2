#include "stdafx.h"
#include "fuellevel.h"

FuelLevel::FuelLevel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

FuelLevel::~FuelLevel()
{}

void FuelLevel::on_lowfueltest_clicked()
{
    setFuelPercentage(6);
    updateFuelLevel();
}
void FuelLevel::on_critfueltest_clicked()
{
    setFuelPercentage(5);
    updateFuelLevel();
}

void FuelLevel::updateFuelLevel()
{
    ui.fuelBar->setValue(calculateFuelPercentage());

    if (isFuelCritical())
    {
        ui.yellowLight->setVisible(false);
        ui.redLight->setVisible(true);

    }
    else if (isFuelLow())
    {
        ui.redLight->setVisible(false);
        ui.yellowLight->setVisible(true);
    }
    else
    {
        ui.redLight->setVisible(false);
        ui.yellowLight->setVisible(false);
    }
}

void FuelLevel::on_up_clicked()
{
    this->currentFuelLevel = currentFuelLevel + 1;
    updateFuelLevel();
}
void FuelLevel::on_down_clicked()
{
    this->currentFuelLevel = currentFuelLevel - 1;
    updateFuelLevel();
}