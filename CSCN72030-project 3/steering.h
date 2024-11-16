#pragma once

#include <QWidget>
#include "ui_Steering.h"

class Steering : public QWidget
{
	Q_OBJECT

	int sliderPosition;
	int wheelAngle;

public:
	Steering(QWidget *parent = nullptr);
	~Steering();
	// steering functions
	int calculateWheelAngle(int sliderPosition)
	{
		this->wheelAngle = sliderPosition * 2;
		return this->wheelAngle;
	}

private slots:
	void getSliderInput(int value);
	void rotateWheel();

private:
	Ui::SteeringClass ui;
};

