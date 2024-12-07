#include "stdafx.h"
#include "ParkingBrake.h"

ParkingBrake::ParkingBrake(QWidget* parent, Speed* spd)
	: QWidget(parent)
{
	this->speed = spd;
	this->state = true;

	ui.setupUi(this);
	update_ParkingBrake_UI();
}

ParkingBrake::~ParkingBrake()
{}

bool ParkingBrake::validateEngage()
{
	if (this->state == false) {
		return (0 == speed->getCurrentSpeed());
	}
	return true;
}

void ParkingBrake::on_ParkingBrakeButton_clicked() {
	if (validateEngage()) {
		this->state = !(this->state);
		update_ParkingBrake_UI();
	}
}

void ParkingBrake::update_ParkingBrake_UI() {
	if (this->state == true) {
		ui.PBIndicator->setVisible(true);
	}
	else if (this->state == false) {
		ui.PBIndicator->setVisible(false);
	}
}