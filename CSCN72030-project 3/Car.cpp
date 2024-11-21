#include "stdafx.h"
#include "Car.h"
#include <qslider.h>
#include <qt>


Car::Car(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CarClass)
{
    ui->setupUi(this);

    // initialize classes
    fl = new FuelLevel(this);
    str = new Steering(this);

    // add class widget to the layout prototype in th ui
    ui->steeringPrototype->addWidget(str);
    ui->fuelLevelPrototype->addWidget(fl);


    // create new thread for file reading and worker to read fuel data
    FuelReader* reader = new FuelReader();
    QThread* fuelThread = new QThread();

    // move reader to thread
    reader->moveToThread(fuelThread);

    // connect thread slots
    connect(fuelThread, &QThread::started, reader, &FuelReader::readFuelData);
    connect(reader, &FuelReader::lineRead, fl, &FuelLevel::updateFuelLevel);
    connect(reader, &FuelReader::lineRead, reader, &FuelReader::deleteLater);
    connect(fuelThread, &QThread::finished, fuelThread, &QThread::deleteLater);

    // start fuel thread
    fuelThread->start();
      
}

Car::~Car()
{
    delete ui;
    delete fl;
    delete str;
}
