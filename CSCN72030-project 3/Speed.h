#pragma once
#include <QWidget>
#include <QSlider>
#include <QTimer>
#include <QMessageBox>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include "ui_SpeedTop.h"
#include "ui_SpeedBottom.h"
#include "fuellevel.h"
#include "Lights.h"
#include "ui_autolights.h"

// SpeedReader class for threading and file reading
class SpeedReader : public QObject
{
    Q_OBJECT

public:
    SpeedReader() {}
    ~SpeedReader() {}

public slots:
    // Slot to handle speed file reading
    void readSpeedData();

signals:
    // Signal emitted when a speed value is read
    void lineRead(float speed);
};

// Speed class for UI and logic
class Speed : public QWidget
{
    Q_OBJECT

private:
    // Timer for speed increments
    QTimer* speedTimer;
    QTimer* fuelTimer; // Timer for fuel checks

    QTimer* speedReductionTimer;

    FuelLevel* fuelLevel;

    Lights* lights;

    // Current speed value
    float currentSpeed;

    // Maximum allowable speed
    float maxSpeed;

    // Flag to check if max speed is set
    bool maxSpeedSet;

    // Step value from accelerate slider
    int accelerateStep;

    // Step value from brake slider
    int brakeStep;

    // Vector to hold speed values
    QVector<float> speedValues;
    
    //FuelLevel* fuel = nullptr
public:
    Speed(QWidget* parent = nullptr, FuelLevel* fuelLevel = nullptr, Lights* lights = nullptr);
    ~Speed();
    QWidget* speedTopWidget() { return m_speedTopWidget; }
    QWidget* speedBottomWidget() { return m_speedBottomWidget; }

    float getCurrentSpeed() const { return currentSpeed; }
    void setCurrentSpeed(float speed);
    // Update speed from external file
    void updateSpeedFromFile(float speed);

private:
    // Top section of Speed UI
    QWidget* m_speedTopWidget;

    // Bottom section of Speed UI
    QWidget* m_speedBottomWidget;

    // UI for SpeedTop
    Ui::SpeedTop topUi;

    // UI for SpeedBottom
    Ui::SpeedBottom bottomUi;

    // Load speed values from file
    void loadSpeedValues();

    // Update speed file
    void updateSpeedFile(float oldSpeed, float newSpeed);



public slots:

    // Slot to handle max speed setting
    void setMaxSpeedButton(); 

   // Slot to handle accelerate slider input
     void getAccelerateSliderInput(int value);

    // Slot to handle brake slider input
    void getBrakeSliderInput(int value);

    // Slot to increment speed based on slider
    void incrementSpeed();

    // Update speed UI display
    void updateSpeedUI();

    void adjustSpeedBasedOnFuel();
};
