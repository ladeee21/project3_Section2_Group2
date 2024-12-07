#pragma once

#include <QWidget>
#include "ui_ParkingBrake.h"
#include "Speed.h"

class ParkingBrake : public QWidget
{
	Q_OBJECT

	Speed* speed;
	bool state;

public:
	ParkingBrake(QWidget* parent = nullptr, Speed* spd = nullptr);
	~ParkingBrake();

	bool validateEngage();

private:
	Ui::ParkingBrakeClass ui;
private slots:
	void on_ParkingBrakeButton_clicked();
	void update_ParkingBrake_UI();
};
