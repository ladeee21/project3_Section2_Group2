#include "stdafx.h"
#include "GearShift.h"

GearShift::GearShift(QWidget* parent, Speed* spd, Lights* lgt)
	: QWidget(parent)
{	
	this->speed = spd;
	this->light = lgt;
	this->gear = 0;

	ui.setupUi(this);
	update_GearShift_UI();
}

GearShift::~GearShift()
{}

bool GearShift::validateShift(int newGear)
{
	if ((newGear == 0) || (newGear == 1) || (newGear == 3)) {
		if (0 == speed->getCurrentSpeed()) {
			return true;
		}
	}
	else if (newGear == 2) {
		return true;
	}
	return false;
}

void GearShift::on_ParkButton_clicked() {
	if (validateShift(0)) {
		this->gear = 0;
		update_GearShift_UI();
		light->updateLightState("reverse light", false);
		light->updateLightUI("reverse light", false);
	}
}
void GearShift::on_ReverseButton_clicked() {
	if (validateShift(1)) {
		this->gear = 1;
		update_GearShift_UI();
		light->updateLightState("reverse light", true);
		light->updateLightUI("reverse light", true);
	}
}
void GearShift::on_NeutralButton_clicked() {
	if (validateShift(2)) {
		this->gear = 2;
		update_GearShift_UI();
		light->updateLightState("reverse light", false);
		light->updateLightUI("reverse light", false);
	}
}
void GearShift::on_DriveButton_clicked() {
	if (validateShift(3)) {
		this->gear = 3;
		update_GearShift_UI();
		light->updateLightState("reverse light", false);
		light->updateLightUI("reverse light", false);
	}
}

void GearShift::update_GearShift_UI() {
	if (this->gear == 0) {
		ui.ParkIndicator->setVisible(true);
		ui.ReverseIndicator->setVisible(false);
		ui.NeutralIndicator->setVisible(false);
		ui.DriveIndicator->setVisible(false);
	}
	else if (this->gear == 1) {
		ui.ParkIndicator->setVisible(false);
		ui.ReverseIndicator->setVisible(true);
		ui.NeutralIndicator->setVisible(false);
		ui.DriveIndicator->setVisible(false);
	}
	else if (this->gear == 2) {
		ui.ParkIndicator->setVisible(false);
		ui.ReverseIndicator->setVisible(false);
		ui.NeutralIndicator->setVisible(true);
		ui.DriveIndicator->setVisible(false);
	}
	else if (this->gear == 3) {
		ui.ParkIndicator->setVisible(false);
		ui.ReverseIndicator->setVisible(false);
		ui.NeutralIndicator->setVisible(false);
		ui.DriveIndicator->setVisible(true);
	}
}
