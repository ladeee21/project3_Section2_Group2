#pragma once

#include <QWidget>
#include <iostream>
#include "ui_Steering.h"

class Steering : public QWidget
{
	Q_OBJECT

	int sliderPosition;
	int wheelAngle;

public:
	Steering(QWidget *parent = nullptr);
	~Steering();

	
	int calculateWheelAngle(int sliderPosition)
	{
		this->wheelAngle = sliderPosition * 2;
		return this->wheelAngle;
	}
	void readSteeringData();
	void writeSteeringData();

private slots:
	void getSliderInput(int value);
	void rotateWheel();

private:
	Ui::SteeringClass ui;
};

