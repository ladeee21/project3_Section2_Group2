#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Car.h"
#include "steering.h"
#include "fuellevel.h"
#include "Lights.h"
#include "Speed.h"
#include "GearShift.h"
#include "ParkingBrake.h"

class Car : public QMainWindow
{
    Q_OBJECT

public:
    Car(QWidget *parent = nullptr);
    ~Car();

  
private:
    Ui::CarClass *ui;
    FuelLevel *fl;
    Steering* str;
    Speed* spd;
    Lights* lgt;
    GearShift* gs;
    ParkingBrake* pb;
};
