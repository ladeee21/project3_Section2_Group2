#pragma once
#include <QWidget>
#include "ui_Speed.h"
#include <QString>
#include <QVector>
#include "fuellevel.h"
#include "spinbox.h"

class Speed : public QWidget
{
    Q_OBJECT

public:
    explicit Speed(QWidget* parent = nullptr);
    ~Speed();
    int getCurrentSpeed() const;
  //  void setFuelLevel(FuelLevel* fuelLevel); // Setter to initialize the FuelLevel

private:
    Ui::SpeedClass ui;

    // Speed-related variables
    int maxSpeedLimit;         // Maximum speed limit
    int currentSpeed;          // Current speed
    int lastValidSpeed;        // Last valid speed before invalid input
    QVector<int> speedValues;  // Valid speed values loaded from the file
    bool maxSpeedSet;          // Indicates whether the maximum speed is set
    CustomSpinBox* currentSpeedInput; // Replace default spinbox with custom spinbox


    // Resource-related variables
    QString resourceFilePath;  // Path to the speed values resource file
    FuelLevel* fuelLevel;      // Pointer to FuelLevel class instance

    // Helper methods
    void loadSpeedValues();       // Load speed values from the resource file
    void configureSpinBox();      // Configure the spin box with valid speed values
    void updateSpeedUI();         // Updates the speed-related UI components
  //  bool validateSpeed(int speed) const; // Checks if a speed is valid based on file values

private slots:
    void setMaxSpeed();                // Sets the maximum speed limit
  //  void spinBoxValueChanged(int value); // Handles spin box navigation (up/down changes)
    void setCurrentSpeed();            // Confirms and sets the current speed manually
    void updateSpeedFile(int oldValue, int newValue); // Updates the speed data file with new values
    void accelerate(); // Handles acceleration
    void brake();      // Handles braking
};
