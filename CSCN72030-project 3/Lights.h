#pragma once
#include "ui_HeadLights.h"
#include "ui_turnsignals.h"
#include "ui_autolights.h"
#include "ui_cabinlight.h"
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
    QWidget* HeadLightsWidget() { return m_HeadLightsWidget; }
    QWidget* AutoLightsWidget(){return m_AutoLightsWidget;}
    QWidget* CabinLightWidget() { return m_CabinLightWidget; }
    QWidget* TurnSignalsWidget() { return m_TurnSignalsWidget; }

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
    Ui::HeadLightsClass Hui;
    Ui::AutoLights Aui;
    Ui::CabinLight Cui;
    Ui::TurnSignals Tui;


    QWidget* m_HeadLightsWidget;
    QWidget* m_AutoLightsWidget;
    QWidget* m_CabinLightWidget;
    QWidget* m_TurnSignalsWidget;
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