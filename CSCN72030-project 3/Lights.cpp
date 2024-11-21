#include "stdafx.h"
#include "Lights.h"
#include <QInputDialog>
#include <QMessageBox>

Lights::Lights(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    // Initialize all lights to OFF
    lightsState.insert("high beams", "OFF");
    lightsState.insert("low beams", "OFF");
    lightsState.insert("turn signal left", "OFF");
    lightsState.insert("turn signal right", "OFF");
    lightsState.insert("reverse light", "OFF");
    lightsState.insert("brake lights", "OFF");
    lightsState.insert("interior cabin light", "OFF");
    lightsState.insert("parking lights", "OFF");

    // Connect button to enter light name
    connect(ui.controlLightButton, &QPushButton::clicked, this, &Lights::onLightNameEntered);

    updateLightsUI();
}

Lights::~Lights() {}

// Slot: User enters the light name
void Lights::onLightNameEntered()
{
    bool ok;
    QString lightName = QInputDialog::getText(this, "Enter Light Name",
        "Which light do you want to control?",
        QLineEdit::Normal, "", &ok);

    if (!ok || lightName.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "You must enter a valid light name.");
        return;
    }

    lightName = lightName.trimmed().toLower();

    if (!validateLightName(lightName)) {
        QMessageBox::warning(this, "Invalid Light", "The light name you entered is not recognized.");
        return;
    }

    // Proceed to ask for light state
    onLightStateEntered(lightName);
}

// Slot: User enters the light state
void Lights::onLightStateEntered(const QString& lightName)
{
    bool ok;
    QString state = QInputDialog::getText(this, "Enter Light State",
        QString("Enter state for '%1' (ON/OFF):").arg(lightName),
        QLineEdit::Normal, "", &ok);

    if (!ok || state.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "You must enter a valid state.");
        return;
    }

    state = state.trimmed().toLower();

    if (!validateLightState(state)) {
        QMessageBox::warning(this, "Invalid State", "State must be either 'ON' or 'OFF'.");
        return;
    }

    // Toggle the light state
    toggleLight(lightName, state);
}

// Helper: Validate the light name
bool Lights::validateLightName(const QString& lightName)
{
    return lightsState.contains(lightName);  // check if the normalized light name is valid
}

// Helper: Validate the light state
bool Lights::validateLightState(const QString& state)
{
    return state == "on" || state == "off"; // validate "on" or "off"
}

// Helper: Toggle the state of a light
void Lights::toggleLight(const QString& lightName, const QString& state)
{
    lightsState[lightName] = state.toUpper();  // store the state in uppercase for consistency

    // Update the UI to reflect the new state
    updateLightsUI();

    QMessageBox::information(this, "Light Updated",
        QString("The state of '%1' has been changed to '%2'.")
        .arg(lightName, state.toUpper()));
}

// System-Controlled Lights
void Lights::activateReverseLight()
{
    lightsState["reverse light"] = "ON";
    updateLightsUI();
}

void Lights::activateBrakeLight()
{
    lightsState["brake lights"] = "ON";
    updateLightsUI();
}

void Lights::activateParkingLight()
{
    lightsState["parking lights"] = "ON";
    updateLightsUI();
}

// Update UI to reflect the state of each light
void Lights::updateLightsUI()
{
    // High Beam
    ui.highBeamLight->setStyleSheet(lightsState["high beams"] == "ON" ? "background-color: blue;" : "");

    // Low Beam
    ui.lowBeamLight->setStyleSheet(lightsState["low beams"] == "ON" ? "background-color: green;" : "");

    // Turn Signal Left
    ui.turnSignalLeft->setPixmap(QPixmap(lightsState["turn signal left"] == "ON" ? ":/icons/turn_left_green.png" : ":/icons/turn_left_white.png"));

    // Turn Signal Right
    ui.turnSignalRight->setPixmap(QPixmap(lightsState["turn signal right"] == "ON" ? ":/icons/turn_right_green.png" : ":/icons/turn_right_white.png"));

    // Interior Cabin Light
    ui.interiorLight->setStyleSheet(lightsState["interior cabin light"] == "ON" ? "background-color: purple;" : "");

    // Reverse Light
    ui.reverseLight->setStyleSheet(lightsState["reverse light"] == "ON" ? "background-color: orange;" : "");

    // Brake Light
    ui.brakeLight->setStyleSheet(lightsState["brake lights"] == "ON" ? "background-color: red;" : "");

    // Parking Light
    ui.parkingLight->setStyleSheet(lightsState["parking lights"] == "ON" ? "background-color: yellow;" : "");
}
