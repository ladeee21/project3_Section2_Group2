#include "stdafx.h"
#include "spinbox.h"

CustomSpinBox::CustomSpinBox(QWidget* parent)
    : QSpinBox(parent)
{
}

void CustomSpinBox::setValidValues(const QVector<int>& values)
{
    validValues = values;
    std::sort(validValues.begin(), validValues.end()); // Ensure values are sorted
}

void CustomSpinBox::stepBy(int steps)
{
    int currentValue = value();

    if (validValues.isEmpty()) {
        QSpinBox::stepBy(steps); // Default behavior if no valid values are set
        return;
    }

    int newValue = currentValue;

    if (steps > 0) { // Step up
        auto it = std::upper_bound(validValues.begin(), validValues.end(), currentValue);
        if (it != validValues.end()) {
            newValue = *it; // Move to the next higher valid value
        }
    }
    else { // Step down
        auto it = std::lower_bound(validValues.begin(), validValues.end(), currentValue);
        if (it != validValues.begin()) {
            --it; // Move to the previous valid value
            newValue = *it;
        }
    }

    setValue(newValue);
}
