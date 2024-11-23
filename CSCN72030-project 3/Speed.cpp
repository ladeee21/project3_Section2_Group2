#include "stdafx.h"
#include "Speed.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

// Constructor
Speed::Speed(QWidget* parent)
    : QWidget(parent), currentSpeed(0), maxSpeed(0), maxSpeedSet(false), accelerateStep(0), brakeStep(0)
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

    // Disable sliders until max speed is set
    bottomUi.accelerateSlider->setValue(0);
    bottomUi.brakeSlider->setValue(0);
    bottomUi.accelerateSlider->setEnabled(false);
    bottomUi.brakeSlider->setEnabled(false);

    // Connect UI elements
    connect(topUi.setMaxSpeedButton, &QPushButton::clicked, this, &Speed::setMaxSpeedButton);
    connect(bottomUi.accelerateSlider, &QSlider::valueChanged, this, &Speed::getAccelerateSliderInput);
    connect(bottomUi.brakeSlider, &QSlider::valueChanged, this, &Speed::getBrakeSliderInput);
    connect(bottomUi.accelerateSlider, &QSlider::valueChanged, this, &Speed::getAccelerateSliderInput);
    connect(bottomUi.brakeSlider, &QSlider::valueChanged, this, &Speed::getBrakeSliderInput);
    connect(bottomUi.accelerateButton, &QPushButton::clicked, this, &Speed::on_accelerateButton_clicked);
    connect(bottomUi.brakeButton, &QPushButton::clicked, this, &Speed::on_brakeButton_clicked);

    // Load speed values from file
    loadSpeedValues();
}

// Destructor
Speed::~Speed()
{
    delete speedTimer;
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

    // Enable the sliders
    bottomUi.accelerateSlider->setEnabled(true);
    bottomUi.brakeSlider->setEnabled(true);
}


// Slot to handle accelerate slider input
void Speed::getAccelerateSliderInput(int value)
{
    if (!maxSpeedSet) {
        QMessageBox::warning(this, "Error", "Please set the maximum speed first.");
        bottomUi.accelerateSlider->setValue(0); // Reset slider
        return;
    }

    // Map slider value to step size
    if (value == 0) {
        accelerateStep = 0;
        speedTimer->stop();
    }
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

    speedTimer->start(3000); // Start a 3-second timer
}

// Slot to handle brake slider input
void Speed::getBrakeSliderInput(int value)
{
    if (!maxSpeedSet) {
        QMessageBox::warning(this, "Error", "Please set the maximum speed first.");
        bottomUi.brakeSlider->setValue(0); // Reset slider
        return;
    }

    // Map slider value to step size
    if (value == 0) {
        brakeStep = 0;
        speedTimer->stop();
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

    speedTimer->start(3000); // Start a 3-second timer
}

// Slot for accelerate button
void Speed::on_accelerateButton_clicked()
{

    if (!maxSpeedSet) {
        QMessageBox::warning(this, "Error", "Set the maximum speed first.");
        return;
    }

    auto it = std::find(speedValues.begin(), speedValues.end(), currentSpeed);
    for (int i = 0; i < 5 && it != speedValues.end(); ++i) {
        ++it;
    }

    if (it == speedValues.end() || *it > maxSpeed) {
        QMessageBox::information(this, "Notice", "Cannot accelerate further.");
        return;
    }

    int newSpeed = *it;
    int oldSpeed = currentSpeed;
    setCurrentSpeed(newSpeed);
    updateSpeedFile(oldSpeed, newSpeed);
    updateSpeedUI();
}

// Slot for brake button
void Speed::on_brakeButton_clicked()
{
    if (!maxSpeedSet) {
        QMessageBox::warning(this, "Error", "Set the maximum speed first.");
        return;
    }

    auto it = std::find(speedValues.begin(), speedValues.end(), currentSpeed);
    for (int i = 0; i < 5 && it != speedValues.begin(); ++i) {
        --it;
    }

    if (it == speedValues.begin()) {
        QMessageBox::information(this, "Notice", "Cannot brake further.");
        return;
    }

    int newSpeed = *it;
    int oldSpeed = currentSpeed;
    setCurrentSpeed(newSpeed);
    updateSpeedFile(oldSpeed, newSpeed);
    updateSpeedUI();
}

// Increment speed based on the slider input
void Speed::incrementSpeed()
{
    int step = (accelerateStep != 0) ? accelerateStep : brakeStep;
    float newSpeed = currentSpeed + step;

    if (newSpeed > maxSpeed) {
        newSpeed = maxSpeed;
        speedTimer->stop();
    }
    else if (newSpeed < 0) {
        newSpeed = 0;
        speedTimer->stop();
    }

    float oldSpeed = currentSpeed;
    setCurrentSpeed(newSpeed);
    updateSpeedFile(oldSpeed, newSpeed);
}

// Update the speed UI
void Speed::updateSpeedUI()
{
    topUi.speedDisplay->setText(QString("Current Speed: %1 km/h").arg(currentSpeed));
}

// Update speed from the external file (called by SpeedReader)
void Speed::updateSpeedFromFile(float speed)
{
    setCurrentSpeed(speed);
}

// Set the current speed value and update the UI
void Speed::setCurrentSpeed(float speed)
{
    currentSpeed = speed;
    updateSpeedUI();
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
    while (!in.atEnd()) {
        int speed = in.readLine().toInt();
        speedValues.append(speed);
    }
    file.close();
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

    file.resize(0);
    QTextStream out(&file);
    for (QString& line : lines) {
        if (line.toFloat() == oldSpeed) {
            out << QString::number(newSpeed) << '\n';
        }
        else {
            out << line << '\n';
        }
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
        emit lineRead(line.toFloat()); // Emit the read line as a float
        QThread::msleep(100);          // Optional: Simulate delay for reading
    }

    file.close();
}


void Speed::decreaseSpeed(FuelLevel* fuellevel) {
    if (fuellevel->isFuelEmpty() == true) {
        setCurrentSpeed(0);
        updateSpeedUI();
    }
}