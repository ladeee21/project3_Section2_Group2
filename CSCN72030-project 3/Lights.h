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
    void updateLightState(const QString& lightName, bool state);
    void updateLightUI(const QString& lightName, bool state);

private slots:
    // Button press slots
    void highBeamButton();
    void lowBeamButton();
    void InteriorCabinButton();
    void toggleTurnSignalRight();
    void toggleTurnSignalLeft();

    // System event slots
    void reverseButton();
    void brakeButton();
   

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
    
    
    // Internal light states
    bool highBeamOn;
    bool lowBeamOn;
    bool turnSignalLeftOn;
    bool turnSignalRightOn;
    bool interiorCabinOn;
    bool reverseLightOn;
    bool brakeLightOn;
};