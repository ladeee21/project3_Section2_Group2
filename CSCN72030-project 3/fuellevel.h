#pragma once

#include <QWidget>
#include "ui_fuellevel.h"
#define MAXFUEL 50.00

class FuelLevel : public QWidget
{
	Q_OBJECT

	float currentFuelLevel;
	bool lowFuelAlert;
	bool criticalFuelAlert;
	int fuelPercentage;

public:
	FuelLevel(QWidget *parent = nullptr);
	~FuelLevel();

	float getFuelLevel()
	{
		return currentFuelLevel;
	}

	void setFuelPercentage(int a)
	{
		this->currentFuelLevel = a;
	}

	int calculateFuelPercentage()
	{
		fuelPercentage = (currentFuelLevel / MAXFUEL) * 100;

		return fuelPercentage;
	}

	bool isFuelLow()
	{
		if (fuelPercentage <= 15 && fuelPercentage > 10)
			return true;
		else return false;
	}

	bool isFuelCritical()
	{
		if (fuelPercentage <= 10)
			return true;
		else return false;
	}

	bool isFuelEmpty()
	{
		if (fuelPercentage == 0)
			return true;
		else return false;
	}

private:
	Ui::FuelLevelClass ui;

private slots:
	void on_lowfueltest_clicked();
	void on_critfueltest_clicked();
	void on_up_clicked();
	void on_down_clicked();
	void updateFuelLevel();
};
