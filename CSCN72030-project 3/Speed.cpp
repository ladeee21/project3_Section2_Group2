#include "stdafx.h"
#include "Speed.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <algorithm>

Speed::Speed(QWidget* parent)
    : QWidget(parent), maxSpeedLimit(0), currentSpeed(0), lastValidSpeed(0), maxSpeedSet(false)
{
    ui.setupUi(this);

    // Replace the default QSpinBox with CustomSpinBox
    currentSpeedInput = new CustomSpinBox(this);
    ui.currentSpeedLayout->replaceWidget(ui.currentSpeedInput, currentSpeedInput); // Replace in the existing layout
    delete ui.currentSpeedInput; // Remove the old spinbox
    ui.currentSpeedInput = currentSpeedInput; // Update pointer in UI for future references

    // Set initial spinbox configuration
    currentSpeedInput->setRange(0, 9999); // Allow manual entry of any value
    currentSpeedInput->setSingleStep(1);  // Default step size for manual use

    // Connect signals
    connect(ui.setMaxSpeedButton, &QPushButton::clicked, this, &Speed::setMaxSpeed);
    connect(ui.setCurrentSpeedButton, &QPushButton::clicked, this, &Speed::setCurrentSpeed);
    connect(currentSpeedInput, QOverload<int>::of(&QSpinBox::valueChanged), this, &Speed::updateSpeedUI);
    connect(ui.accelerateButton, &QPushButton::clicked, this, &Speed::accelerate);
    connect(ui.brakeButton, &QPushButton::clicked, this, &Speed::brake);

    // Load speed values and configure spinbox
    loadSpeedValues();
    configureSpinBox();
    updateSpeedUI();
}

Speed::~Speed() {}

void Speed::loadSpeedValues()
{
    QFile file(":/data/speed_data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to load speed data file.");
        return;
    }

    QTextStream in(&file);
    speedValues.clear();
    while (!in.atEnd()) {
        int value = in.readLine().toInt();
        if (value > 0 && value <= 500) {
            speedValues.append(value);
        }
    }
    file.close();

    std::sort(speedValues.begin(), speedValues.end());
}

void Speed::configureSpinBox()
{
    if (speedValues.isEmpty()) {
        currentSpeedInput->setEnabled(false);
        return;
    }

    currentSpeedInput->setEnabled(true);
    currentSpeedInput->setValidValues(speedValues); // Set valid values for stepping

    // Initialize spinbox with the first valid speed
    currentSpeedInput->setValue(*speedValues.begin());
}

void Speed::setMaxSpeed()
{
    bool ok;
    int input = ui.maxSpeedInput->text().toInt(&ok);

    if (!ok || input <= 0 || input > 220) {
        QMessageBox::warning(this, "Invalid Input", "Max speed cannot exceed 220 or be zero.");
        return;
    }

    maxSpeedLimit = input;
    maxSpeedSet = true;
    QMessageBox::information(this, "Success", "Maximum speed set successfully.");
}

void Speed::setCurrentSpeed()
{
    int value = currentSpeedInput->value();

    if (!maxSpeedSet) {
        QMessageBox::warning(this, "Error", "Set the maximum speed first.");
        return;
    }

    if (value > maxSpeedLimit) {
        QMessageBox::warning(this, "Error", "Current speed exceeds maximum speed limit.");
        return;
    }

    if (!speedValues.contains(value)) {
        QMessageBox::warning(this, "Error", "Selected speed is not in the file.");
        return;
    }

    // Store the current speed before changing it
    int oldSpeed = currentSpeed;

    currentSpeed = value;
    updateSpeedFile(oldSpeed, currentSpeed);  // Update the file by replacing the old speed with the new one

    QMessageBox::information(this, "Success", "Current speed set successfully.");
    updateSpeedUI();
}

void Speed::updateSpeedUI()
{
    // Store the old current speed before updating
    int oldSpeed = currentSpeed;

    // Update the UI with the new current speed
    ui.speedDisplay->setText(QString("Current Speed: %1 km/h").arg(currentSpeed));

    // Now update the file with the old and new current speed
    updateSpeedFile(oldSpeed, currentSpeed);
}

int Speed::getCurrentSpeed() const
{
    return currentSpeed;
}

void Speed::updateSpeedFile(int oldValue, int newValue)
{
    QFile file("speed_data.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open speed data file for writing.");
        return;
    }

    QTextStream in(&file);
    QStringList fileLines;

    // Read the existing lines from the file
    while (!in.atEnd()) {
        fileLines.append(in.readLine());
    }

    // Update the file by replacing the old speed with the new one
    for (int i = 0; i < fileLines.size(); ++i) {
        if (fileLines[i].toInt() == oldValue) {
            fileLines[i] = QString::number(newValue);  // Replace old speed with new one
            break;
        }
    }

    // Overwrite the file with the updated speeds
    file.resize(0);  // Clear the file before writing
    QTextStream out(&file);
    for (const QString& line : fileLines) {
        out << line << "\n";
    }

    file.close();
}

void Speed::accelerate()
{
    if (!maxSpeedSet) {
        QMessageBox::warning(this, "Error", "Set the maximum speed first.");
        return;
    }

    // Find the 5th higher value in the speed file
    auto it = std::find(speedValues.begin(), speedValues.end(), currentSpeed);
    for (int i = 0; i < 5 && it != speedValues.end(); ++i) {
        ++it;
    }

    if (it == speedValues.end()) {
        QMessageBox::information(this, "Notice", "Cannot accelerate further.");
        return;
    }

    int newSpeed = *it;
    int oldSpeed = currentSpeed;  // Save old speed
    currentSpeed = newSpeed;

    // Update the file
    updateSpeedFile(oldSpeed, newSpeed);

    // Update UI
    updateSpeedUI();
    QMessageBox::information(this, "Success", "Accelerated to a higher speed.");
}

void Speed::brake()
{
    if (!maxSpeedSet) {
        QMessageBox::warning(this, "Error", "Set the maximum speed first.");
        return;
    }

    // Find the 5th lower value in the speed file
    auto it = std::find(speedValues.begin(), speedValues.end(), currentSpeed);
    for (int i = 0; i < 5 && it != speedValues.begin(); ++i) {
        --it;
    }

    if (it == speedValues.begin()) {
        QMessageBox::information(this, "Notice", "Cannot brake further.");
        return;
    }

    int newSpeed = *it;
    int oldSpeed = currentSpeed;  // Save old speed
    currentSpeed = newSpeed;

    // Update the file
    updateSpeedFile(oldSpeed, newSpeed);

    // Update UI
    updateSpeedUI();
    QMessageBox::information(this, "Success", "Braked to a lower speed.");
}
