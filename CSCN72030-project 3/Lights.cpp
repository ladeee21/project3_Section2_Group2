#include "stdafx.h"
#include "Lights.h"
#include <QtUiTools/quiloader.h>
#include <QFile>
#include <QVBoxLayout>
#include <QDebug>

Lights::Lights(QWidget* parent)
    : QWidget(parent),
    m_HeadLightsWidget(new QWidget(this)),
    m_AutoLightsWidget(new QWidget(this)),
    m_CabinLightWidget(new QWidget(this)),
    m_TurnSignalsWidget(new QWidget(this))
{
    // Set up the HeadLights widget
    Hui.setupUi(m_HeadLightsWidget);

    // Set up the AutoLights widget
    Aui.setupUi(m_AutoLightsWidget);

    // Set up the CabinLight widget
    Cui.setupUi(m_CabinLightWidget);

    // Set up the TurnSignals widget
    Tui.setupUi(m_TurnSignalsWidget);

    // Connect the signals to appropriate slots
    connect(Hui.highBeamButton, &QPushButton::clicked, this, &Lights::highBeamButton);
    connect(Hui.lowBeamButton, &QPushButton::clicked, this, &Lights::lowBeamButton);
    connect(Cui.InteriorCabinButton, &QPushButton::clicked, this, &Lights::InteriorCabinButton);
    connect(Aui.reverseButton, &QPushButton::clicked, this, &Lights::reverseButton);
    connect(Aui.brakeButton, &QPushButton::clicked, this, &Lights::brakeButton);
    connect(Tui.turnSignalLeft, &QPushButton::clicked, this, &Lights::toggleTurnSignalLeft);
    connect(Tui.turnSignalRight, &QPushButton::clicked, this, &Lights::toggleTurnSignalRight);

    initializeLightStates();
}
    


Lights::~Lights() {
    delete m_HeadLightsWidget;
    delete m_AutoLightsWidget;
    delete m_CabinLightWidget;
    delete m_TurnSignalsWidget;

}


void Lights::initializeLightStates()
{
    QFile file("lights_data.txt");

    // If the file doesn't exist, show an error and exit initialization
    if (!file.exists()) {
        QMessageBox::critical(this, "Error", "The lights_data.txt file is missing. Please ensure the file exists.");
        return;
    }

    // Read the file and update the UI
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(':');
            if (parts.size() != 2) continue;

            QString lightName = parts[0].trimmed();
            QString state = parts[1].trimmed();

            bool isOn = (state == "ON");

            // Update the light's UI and internal state
            updateLightUI(lightName, isOn);
        }
        file.close();
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to open lights_data.txt. Please check file permissions.");
    }
}

void Lights::highBeamButton()
{
    highBeamOn = !highBeamOn;
    updateLightUI("high beams", highBeamOn);
    updateLightState("high beams", highBeamOn);
}

void Lights::lowBeamButton()
{
    lowBeamOn = !lowBeamOn;
    updateLightUI("low beams", lowBeamOn);
    updateLightState("low beams", lowBeamOn);
}

void Lights::InteriorCabinButton()
{
    interiorCabinOn = !interiorCabinOn;
    updateLightUI("interior cabin light", interiorCabinOn);
    updateLightState("interior cabin light", interiorCabinOn);
}


void Lights::toggleTurnSignalLeft()
{
    turnSignalLeftOn = !turnSignalLeftOn;
    updateLightUI("turn signal left", turnSignalLeftOn);
    updateLightState("turn signal left", turnSignalLeftOn);
}

void Lights::toggleTurnSignalRight()
{
    turnSignalRightOn = !turnSignalRightOn;
    updateLightUI("turn signal right", turnSignalRightOn);
    updateLightState("turn signal right", turnSignalRightOn);
}


// System event-based light changes
void Lights::reverseButton()
{
    reverseLightOn = !reverseLightOn;
    updateLightUI("reverse light", reverseLightOn);
    updateLightState("reverse light", reverseLightOn);
}

void Lights::brakeButton()
{
    brakeLightOn = !brakeLightOn;
    updateLightUI("brake light", brakeLightOn);
    updateLightState("brake light", brakeLightOn);
}

void Lights::updateLightState(const QString& lightName, bool state)
{

    qDebug() << "Updating light state:" << lightName << "to" << (state ? "ON" : "OFF");

    // Read the current file contents
    QFile file("lights_data.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString fileContents = in.readAll();  // Read all the file contents
        file.close();

        // Replace the line that corresponds to the light name with the new state
        QString oldState = lightName + ":ON";
        QString newState = lightName + ":" + (state ? "ON" : "OFF");

        // If the light's current state is different from the new state, update it
        if (fileContents.contains(oldState)) {
            fileContents.replace(oldState, newState);  // Replace old state with the new state
        }
        else if (fileContents.contains(lightName + ":OFF")) {
            fileContents.replace(lightName + ":OFF", newState);  // If OFF state, update it to ON/OFF
        }

        // Open the file in WriteOnly mode to overwrite the contents
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << fileContents;  // Write the modified contents back to the file
            file.close();
        }
    }
}

void Lights::updateLightUI(const QString& lightName, bool isOn)
{
    if (lightName == "high beams") {
        highBeamOn = isOn;
        Hui.highBeamButton->setStyleSheet(isOn ? "background-color: blue;" : "background-color: white;");
    }
    else if (lightName == "low beams") {
        lowBeamOn = isOn;
        Hui.lowBeamButton->setStyleSheet(isOn ? "background-color: green;" : "background-color: white;");
    }
    else if (lightName == "turn signal left") {
        turnSignalLeftOn = isOn;
        Tui.turnSignalLeft->setIcon(QIcon(isOn ? ":/icons/turn_left_green.png" : ":/icons/turn_left_white.png"));
    }
    else if (lightName == "turn signal right") {
        turnSignalRightOn = isOn;
        Tui.turnSignalRight->setIcon(QIcon(isOn ? ":/icons/turn_right_green.png" : ":/icons/turn_right_white.png"));
    }
    else if (lightName == "interior cabin light") {
        interiorCabinOn = isOn;
        Cui.InteriorCabinButton->setStyleSheet(isOn ? "background-color: purple;" : "background-color: white;");
    }
    else if (lightName == "reverse light") {
        reverseLightOn = isOn;
        Aui.reverseButton->setStyleSheet(isOn ? "background-color: orange;" : "background-color: white;");
    }
    else if (lightName == "brake light") {
        brakeLightOn = isOn;
        Aui.brakeButton->setStyleSheet(isOn ? "background-color: red;" : "background-color: white;");
    }
 
}
