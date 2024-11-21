#include "stdafx.h"
#include "steering.h"
#include <qslider.h>

Steering::Steering(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    // initialize variables to empty
    this->wheelAngle = 0;
    this->sliderPosition = 0;

    // create slider pointer for slider in ui
    QSlider* mySlider = findChild<QSlider*>("wheelSlider");

    // call getSliderInput whenever the slider is moved
    connect(mySlider, &QSlider::valueChanged, this, &Steering::getSliderInput);
}

void Steering::getSliderInput(int value)
{

    this->sliderPosition = value;
    writeSteeringData();
}

void Steering::writeSteeringData()
{
    // open file in write mode 
    QFile file("steeringFile.txt");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file";
        return;
    }

    // write slider position in file
    QTextStream out(&file);
    out << this->sliderPosition;
    file.close();

    readSteeringData();
}

void Steering::readSteeringData()
{
    // open file in read mode
    QFile file("steeringFile.txt");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file";
        return;
    }
    // read line
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        // change text input to int and set new wheel angle
        this->wheelAngle = calculateWheelAngle(line.toInt());
        rotateWheel();
    }

    file.close();
}

void Steering::rotateWheel()
{
    // declare image location
    QPixmap pixmap(":/icons/steering_wheel.png");

    QTransform transform;
    // rotate image based on wheel angle
    transform.rotate(this->wheelAngle); 

    // display rotated image in ui
    QPixmap rotatedPixmap = pixmap.transformed(transform);
    ui.wheel->setPixmap(rotatedPixmap);

}

Steering::~Steering()
{
}
