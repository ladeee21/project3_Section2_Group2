#include "stdafx.h"
#include "Car.h"
#include <qslider.h>
#include <qt>

Car::Car(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CarClass)
{
    ui->setupUi(this);
}

//void Car::updateSteeringIcon(QPixmap rotatedPixmap)
//{
//    ui->wheel->setPixmap(rotatedPixmap);
//}

Car::~Car()
{
    delete ui;
}
