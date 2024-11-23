#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Car.h"
#include "steering.h"
#include "fuellevel.h"
#include "Lights.h"
#include "Speed.h"

class Car : public QMainWindow
{
    Q_OBJECT

public:
    Car(QWidget *parent = nullptr);
    ~Car();

   // void updateSteeringIcon(QPixmap rotatedPixmap);
private:
    Ui::CarClass *ui;
    FuelLevel *fl;
    Steering* str;
    Speed* spd;
};
