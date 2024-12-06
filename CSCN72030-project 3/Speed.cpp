#include "stdafx.h"
#include "Speed.h"
#include <cmath>  
#include <QFile>
#include <QTextStream>
#include <QDebug>

// Constructor
Speed::Speed(QWidget* parent, FuelLevel* fuelLevel)
    : QWidget(parent), currentSpeed(0), maxSpeed(0), maxSpeedSet(false), accelerateStep(0), brakeStep(0), fuelLevel(fuelLevel)
{
    // Create a layout to hold both widgets
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Initialize SpeedTop widget and UI
    m_speedTopWidget = new QWidget(this);
    topUi.setupUi(m_speedTopWidget);
    mainLayout->addWidget(m_speedTopWidget);

    // Initialize SpeedBottom widget and UI
    m_speedBottomWidget = new QWidget(this);
    bottomUi.setupUi(m_speedBottomWidget);
    mainLayout->addWidget(m_speedBottomWidget);

    // Set the layout for the Speed class
    setLayout(mainLayout);

    // Initialize the timer
    speedTimer = new QTimer(this);
    connect(speedTimer, &QTimer::timeout, this, &Speed::incrementSpeed);

    // Initialize the speed reduction timer
    speedReductionTimer = new QTimer(this);
    connect(speedReductionTimer, &QTimer::timeout, this, &Speed::adjustSpeedBasedOnFuel);
    speedReductionTimer->start(1000); // Adjust speed every second


    // Disable sliders until max speed is set
    bottomUi.accelerateSlider->setValue(0);
    bottomUi.brakeSlider->setValue(0);

    // Connect UI elements
    connect(topUi.setMaxSpeedButton, &QPushButton::clicked, this, &Speed::setMaxSpeedButton);
    connect(bottomUi.accelerateSlider, &QSlider::valueChanged, this, &Speed::getAccelerateSliderInput);
    connect(bottomUi.brakeSlider, &QSlider::valueChanged, this, &Speed::getBrakeSliderInput);

    // Load speed values from file
    loadSpeedValues();

    qDebug() << "Speed class initialized";
}

// Destructor
Speed::~Speed()
{
    delete speedTimer;
    delete speedReductionTimer;
}


// Slot to handle max speed setting
void Speed::setMaxSpeedButton()
{
    if (maxSpeedSet) {
        // If max speed has already been set, no need to show the message again.
        QMessageBox::warning(this, "Notice", "Maximum speed has already been set.");
        return;
    }

    bool ok;
    float inputSpeed = topUi.maxSpeedInput->text().toFloat(&ok);

    if (!ok || inputSpeed <= 0 || inputSpeed > 220) {
        QMessageBox::warning(this, "Error", "Enter a valid maximum speed (1 to 220).");
        return;
    }

    maxSpeed = inputSpeed;
    maxSpeedSet = true; // Set the flag to prevent multiple sets.

    // Show the success message
    QMessageBox::information(this, "Success", QString("Maximum speed set to %1 km/h.").arg(maxSpeed));

}

// Slot to handle accelerate slider input
void Speed::getAccelerateSliderInput(int value)
{
    qDebug() << "Raw Accelerate Slider Value:" << value;

    if (!maxSpeedSet) {
        bottomUi.accelerateSlider->blockSignals(true);
        bottomUi.accelerateSlider->setValue(0);
        bottomUi.accelerateSlider->blockSignals(false);
        QMessageBox::warning(this, "Error", "Please set the maximum speed first.");
        return;
    }

    if (bottomUi.brakeSlider->value() != 0) {
        bottomUi.accelerateSlider->blockSignals(true);
        bottomUi.accelerateSlider->setValue(0);
        bottomUi.accelerateSlider->blockSignals(false);
        QMessageBox::warning(this, "Warning", "Brake slider must be 0 before the accelerate slider can be used.");
        return;
    }

    if (value == 0) {
        accelerateStep = 0;
        speedTimer->stop();
        qDebug() << "Accelerate Step set to 0, Timer stopped";
        return;
    }
    else if (value == 1) {
        accelerateStep = 1;
    }
    // Map slider value to step size
    else if (value == bottomUi.accelerateSlider->maximum()) {
        accelerateStep = 5;
    }
    else if (value > bottomUi.accelerateSlider->maximum() / 2) {
        accelerateStep = 4;
    }
    else if (value == bottomUi.accelerateSlider->maximum() / 2) {
        accelerateStep = 3;
    }
    else {
        accelerateStep = 2;
    }

    // Calculate the new speed
    float newSpeed = currentSpeed + accelerateStep;

    // Check if the new speed exceeds the maximum speed
    if (newSpeed > maxSpeed) {
        QMessageBox::warning(this, "Speed Limit Exceeded",
            QString("Current speed exceeds maximum speed limit of %1 km/h.").arg(maxSpeed));
        bottomUi.accelerateSlider->blockSignals(true);
        bottomUi.accelerateSlider->setValue(0); // Reset the slider to 0
        bottomUi.accelerateSlider->blockSignals(false);
        accelerateStep = 0;
        return;
    }

    speedTimer->start(3000);
    qDebug() << "Timer started with Accelerate Step:" << accelerateStep;
}

// Slot to handle brake slider input
void Speed::getBrakeSliderInput(int value)
{
    if (!maxSpeedSet) {
        bottomUi.brakeSlider->blockSignals(true);
        bottomUi.brakeSlider->setValue(0);
        bottomUi.brakeSlider->blockSignals(false);
        QMessageBox::warning(this, "Error", "Please set the maximum speed first.");
        return;
    }

    if (bottomUi.accelerateSlider->value() != 0) {
        bottomUi.brakeSlider->blockSignals(true);
        bottomUi.brakeSlider->setValue(0);
        bottomUi.brakeSlider->blockSignals(false);
        QMessageBox::warning(this, "Warning", "Accelerate slider must be 0 before the brake slider can be used.");
        return;
    }

    // Map slider value to step size
    if (value == 0) {
        brakeStep = 0;
        speedTimer->stop();
        return;
    }
    else if (value == 1) {
        brakeStep = -1;
    }
    else if (value == bottomUi.brakeSlider->maximum()) {
        brakeStep = -5;
    }
    else if (value > bottomUi.brakeSlider->maximum() / 2) {
        brakeStep = -4;
    }
    else if (value == bottomUi.brakeSlider->maximum() / 2) {
        brakeStep = -3;
    }
    else {
        brakeStep = -2;
    }

    // Check if the new speed would fall below 0
    float newSpeed = currentSpeed + brakeStep;
    if (newSpeed < 0) {
        QMessageBox::warning(this, "Invalid Brake Action", "Brake cannot reduce speed below 0.");
        bottomUi.brakeSlider->blockSignals(true);
        bottomUi.brakeSlider->setValue(0); // Reset slider to 0
        bottomUi.brakeSlider->blockSignals(false);
        brakeStep = 0;
        return;
    }

    speedTimer->start(1000);
}

// Increment speed based on the slider input
void Speed::incrementSpeed()
{
    int step = (accelerateStep != 0) ? accelerateStep : brakeStep;
    float newSpeed = currentSpeed + step;

    qDebug() << "Current Speed:" << currentSpeed << "Step:" << step << "New Speed:" << newSpeed;

    float oldSpeed = currentSpeed;
    setCurrentSpeed(newSpeed);
    updateSpeedFile(oldSpeed, newSpeed);

    // Check if the new speed exceeds the maximum speed
    if (newSpeed > maxSpeed) {
        // Show the warning message
        QMessageBox::warning(this, "Speed Limit Exceeded",
            QString("Current speed exceeds maximum speed limit of %1 km/h.").arg(maxSpeed));

        // Revert to the previous speed
        setCurrentSpeed(oldSpeed);
        updateSpeedFile(newSpeed, oldSpeed);

        // Stop the timer and reset the step to prevent further increments
        speedTimer->stop();
        accelerateStep = 0;
        brakeStep = 0;

        // Reset the slider values to 0
        bottomUi.accelerateSlider->blockSignals(true);
        bottomUi.accelerateSlider->setValue(0);
        bottomUi.accelerateSlider->blockSignals(false);

        bottomUi.brakeSlider->blockSignals(true);
        bottomUi.brakeSlider->setValue(0);
        bottomUi.brakeSlider->blockSignals(false);
    }
    else if (newSpeed < 0) {
        // Prevent the speed from going below 0
        newSpeed = 0;
        speedTimer->stop();
        qDebug() << "New Speed below 0, Timer stopped";

        // Show the warning message
        QMessageBox::warning(this, "Invalid Brake Action", "Brake cannot reduce speed below 0.");

        // Reset the brake slider to 0
        bottomUi.brakeSlider->blockSignals(true);
        bottomUi.brakeSlider->setValue(0);
        bottomUi.brakeSlider->blockSignals(false);

        // Update the speed display to show 0
        setCurrentSpeed(newSpeed);
    }
}

// Update the speed UI
void Speed::updateSpeedUI()
{
    topUi.speedDisplay->setText(QString("Current Speed: %1 km/h").arg(currentSpeed));
    topUi.speedDisplay->adjustSize();
}

// Update speed from the external file (called by SpeedReader)
void Speed::updateSpeedFromFile(float speed)
{
    setCurrentSpeed(speed);
}

// Set the current speed value and update the UI
void Speed::setCurrentSpeed(float speed)
{
    // Update current speed with the float value
    currentSpeed = speed;

    updateSpeedUI(); 

    // Reset sliders only if speed is set to zero
    if (currentSpeed == 0) {
        bottomUi.accelerateSlider->setValue(0);
        bottomUi.brakeSlider->setValue(0);
    }
}


// Load the speed values from the file
void Speed::loadSpeedValues()
{
    QFile file("speed_data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open speed data file.";
        return;
    }

    QTextStream in(&file);
    speedValues.clear();

    while (!in.atEnd()) {
        int speed = in.readLine().toInt();
        speedValues.append(speed);
    }

    file.close();

    // Ensure the file has 0 at the top for initial display
    if (!speedValues.contains(0)) {
        speedValues.prepend(0);
        updateSpeedFile(-1, 0);
    }

    // Set the current speed to 0
    setCurrentSpeed(0);
}

// Update the speed file with new speed value
void Speed::updateSpeedFile(float oldSpeed, float newSpeed)
{
    QFile file("speed_data.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qWarning() << "Could not open speed data file.";
        return;
    }

    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }

    file.resize(0); // Clear the file
    QTextStream out(&file);

    bool replaced = false;
    for (QString& line : lines) {
        if (line.toFloat() == oldSpeed) {
            out << QString::number(newSpeed) << '\n';
            replaced = true;
        }
        else {
            out << line << '\n';
        }
    }

    // If oldSpeed wasn't found (e.g., adding 0 at startup), append it
    if (!replaced && oldSpeed == -1) {
        out << QString::number(newSpeed) << '\n';
    }

    file.close();
}


// SpeedReader implementation: read the speed data from file and emit the signal
void SpeedReader::readSpeedData()
{
    QFile file("speed_data.txt"); // Ensure this file path is correct
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open speed data file.";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        emit lineRead(line.toFloat()); 
        QThread::msleep(100);          
    }

    file.close();
}

// Adjust speed based on fuel level
void Speed::adjustSpeedBasedOnFuel()
{
    qDebug() << "Adjusting speed based on fuel level";

    float currentFuelLevel = fuelLevel->getFuelLevel();
    int fuelPercentage = fuelLevel->calculateFuelPercentage();

    qDebug() << "Current fuel level:" << currentFuelLevel;
    qDebug() << "Fuel percentage:" << fuelPercentage;

    // Check if the fuel is low, critical, or empty
    if (fuelLevel->isFuelEmpty()) {
        qDebug() << "Fuel is empty. Speed should be 0.";
        setCurrentSpeed(0); // If fuel is empty, set speed to 0
        return;
    }

    // Reduce speed if fuel is low or critical
    if (fuelLevel->isFuelLow() || fuelLevel->isFuelCritical()) {
        if (!speedTimer->isActive()) {
            speedTimer->start(1000); // Adjust speed every second
        }

        // Gradually decrease speed by 5% of the current speed each second
        float speedDecrease = (currentSpeed * 0.05); // Reduce speed by 5% every second
        float newSpeed = currentSpeed - speedDecrease;

        // Ensure speed doesn't go below 0
        if (newSpeed < 0) {
            newSpeed = 0;
        }

        // Format the new speed to 1 decimal place
        QString formattedSpeed = QString::number(newSpeed, 'f', 1); // 1 decimal place

        // Apply the formatted speed
        setCurrentSpeed(formattedSpeed.toFloat()); // Update speed as a float
        updateSpeedUI();
        qDebug() << "Speed adjusted to:" << formattedSpeed;
    }
    else {
        qDebug() << "Fuel is sufficient. No speed adjustment.";
    }
}

