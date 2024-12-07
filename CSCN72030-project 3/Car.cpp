#include "stdafx.h"
#include "Car.h"
#include <qslider.h>
#include <qt>

Car::Car(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::CarClass)
{
    ui->setupUi(this);

    // Initialize classes
    fl = new FuelLevel(this);
    str = new Steering(this);
    lgt = new Lights(this);
    spd = new Speed(this,fl, lgt) ; // Pass FuelLevel instance to Speed
   


    //fl
    gs = new GearShift(this, spd, lgt);
    pb = new ParkingBrake(this, spd);
    // Add class widget to the layout prototype in the UI
    ui->steeringPrototype->addWidget(str);
    ui->fuelLevelPrototype->addWidget(fl);
    ui->speedTopPrototype->addWidget(spd->speedTopWidget());
    ui->speedBottomPrototype->addWidget(spd->speedBottomWidget());

    ui->headLightsPrototype->layout()->addWidget(lgt->HeadLightsWidget());
    ui->autoLightsPrototype->layout()->addWidget(lgt->AutoLightsWidget());
    ui->cabinLightPrototype->layout()->addWidget(lgt->CabinLightWidget());
    ui->turnSignalsPrototype->layout()->addWidget(lgt->TurnSignalsWidget());

    ui->GearShiftPrototype->addWidget(gs);
    ui->ParkingBrakePrototype->addWidget(pb);

    // Create new thread for file reading and worker to read fuel data
    FuelReader* reader = new FuelReader();
    QThread* fuelThread = new QThread();

    // Move reader to thread
    reader->moveToThread(fuelThread);

    // Connect thread slots
    connect(fuelThread, &QThread::started, reader, &FuelReader::readFuelData);
    connect(reader, &FuelReader::lineRead, fl, &FuelLevel::updateFuelLevel);
    connect(reader, &FuelReader::lineRead, reader, &FuelReader::deleteLater);
    connect(fuelThread, &QThread::finished, fuelThread, &QThread::deleteLater);

    // Connect fuel level changes to speed adjustments
    connect(fl, &FuelLevel::fuelLevelChanged, spd, &Speed::adjustSpeedBasedOnFuel);

    // Start fuel thread
    fuelThread->start();

    qDebug() << "Car class initialized";
}

Car::~Car()
{
    delete ui;
    delete fl;
    delete str;
    delete spd;
    delete lgt;
    delete gs;
    delete pb;
}
