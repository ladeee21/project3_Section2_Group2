#pragma once
#include <QSpinBox>
#include <QVector>
#include <algorithm>

class CustomSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    explicit CustomSpinBox(QWidget* parent = nullptr);

    void setValidValues(const QVector<int>& values); // Set the valid values for stepping

protected:
    void stepBy(int steps) override; // Override step behavior for up/down buttons

private:
    QVector<int> validValues; // List of valid speed values
};
