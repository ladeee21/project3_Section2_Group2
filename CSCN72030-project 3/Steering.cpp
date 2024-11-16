#include "stdafx.h"
#include "steering.h"
#include <qslider.h>

Steering::Steering(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    this->wheelAngle = 0;
    this->sliderPosition = 0;
    QSlider* mySlider = findChild<QSlider*>("wheelSlider");

    // Connect the slider's valueChanged signal to the updateSliderValue slot
    connect(mySlider, &QSlider::valueChanged, this, &Steering::getSliderInput);
}

void Steering::getSliderInput(int value)
{
    this->sliderPosition = value;
    rotateWheel();
}

void Steering::rotateWheel()
{
    this->wheelAngle = calculateWheelAngle(this->sliderPosition);


    QPixmap pixmap(":/icons/steering_wheel.png"); // Load your image

    QTransform transform;
    transform.rotate(wheelAngle); // Rotate the image by 45 degrees
    QPixmap rotatedPixmap = pixmap.transformed(transform);

    ui.wheel->setPixmap(rotatedPixmap);

}

Steering::~Steering()
{
}
