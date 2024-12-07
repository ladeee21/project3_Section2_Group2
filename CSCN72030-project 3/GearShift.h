#pragma once

#include <QWidget>
#include "ui_GearShift.h"
#include "Lights.h"
#include "Speed.h"

class GearShift : public QWidget
{
	Q_OBJECT

	Speed* speed;
	Lights* light;
	int gear;

public:
	GearShift(QWidget* parent = nullptr, Speed* spd = nullptr, Lights* lgt = nullptr);
	~GearShift();

	bool validateShift(int newGear);

private:
	Ui::GearShiftClass ui;
private slots:
	void on_ParkButton_clicked();
	void on_ReverseButton_clicked();
	void on_NeutralButton_clicked();
	void on_DriveButton_clicked();
	void update_GearShift_UI();
};
