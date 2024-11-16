#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Car.h"

class Car : public QMainWindow
{
    Q_OBJECT

public:
    Car(QWidget *parent = nullptr);
    ~Car();

   // void updateSteeringIcon(QPixmap rotatedPixmap);
protected:
    Ui::CarClass *ui;
};
