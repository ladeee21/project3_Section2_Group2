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

Car::~Car()
{
    delete ui;
}
