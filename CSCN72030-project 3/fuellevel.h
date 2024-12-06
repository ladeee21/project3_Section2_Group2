#pragma once

#include <QWidget>
#include <iostream>
#include "ui_fuellevel.h"
#define MAXFUEL 50.0
using namespace std;

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
		this->fuelPercentage = calculateFuelPercentage();
	}

	int calculateFuelPercentage()
	{
		return( int((currentFuelLevel / MAXFUEL) * 100));
	}

	// if fuel is between 15 and 10 fuel is low
	bool isFuelLow()
	{
		if (fuelPercentage <= 15 && fuelPercentage > 10)
			return true;
		else return false;
	}

	// if fuel is 10 or less fuel is critically low
	bool isFuelCritical()
	{
		if (fuelPercentage <= 10)
			return true;
		else return false;
	}

	// if fuel is 0 return true
	bool isFuelEmpty()
	{
		if (fuelPercentage == 0)
			return true;
		else return false;
	}
	

private:
	Ui::FuelLevelClass ui;

public slots:
	void updateFuelLevel(float fuelLevel);
	void updateFuelPercentageLabel();

signals:
		void fuelLevelChanged();
};

// second class to run the file thread
class FuelReader : public QObject {
	Q_OBJECT
public:
	FuelReader() {}
	~FuelReader() {}

public slots:
	void readFuelData();

signals:
	void lineRead(float fuelLevel);

	
};