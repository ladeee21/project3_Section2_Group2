#pragma once
#include <QWidget>
#include <QString>
#include <QMap>
#include <QPushButton>
#include "ui_lights.h"

class Lights : public QWidget
{
    Q_OBJECT

public:
    explicit Lights(QWidget* parent = nullptr);
    ~Lights();

    // Methods for system-controlled lights
    void activateReverseLight();
    void activateBrakeLight();
    void activateParkingLight();

private:
    Ui::LightsClass ui;

    // Map to track the state of all lights
    QMap<QString, QString> lightsState;

    // Helper functions
    void toggleLight(const QString& lightName, const QString& state);
    bool validateLightName(const QString& lightName); // Validate light name input
    bool validateLightState(const QString& state);    // Validate state input

    // Update UI to reflect the state of each light
    void updateLightsUI();

private slots:
    void onLightNameEntered();  // Slot triggered when user enters the light name
    void onLightStateEntered(const QString& lightName); // Slot triggered when user enters the light state
};
