#pragma once
#include "ui_Lights.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QTextStream>

namespace Ui {
    class LightsClass;
}

class Lights : public QWidget
{
    Q_OBJECT

public:
    explicit Lights(QWidget* parent = nullptr);
    ~Lights();
    void initializeLightStates();

private slots:
    // Button press slots
    void on_highBeamButton_clicked();
    void on_lowBeamButton_clicked();
    void on_InteriorCabinButton_clicked();

    // System event slots
    void on_reverseButton_clicked();
    void on_brakeButton_clicked();
    void on_parkingButton_clicked();
    void toggleTurnSignalRight();
    void toggleTurnSignalLeft();

private:
    Ui::LightsClass ui;  // Direct reference (not pointer) to the UI

    // Helper functions
    void updateLightState(const QString& lightName, bool state);
    void updateLightUI(const QString& lightName, bool state);

    // Internal light states
    bool highBeamOn;
    bool lowBeamOn;
    bool turnSignalLeftOn;
    bool turnSignalRightOn;
    bool interiorCabinOn;
    bool reverseLightOn;
    bool brakeLightOn;
    bool parkingLightOn;
};