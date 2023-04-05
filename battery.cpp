/**
 * https://cdn-shop.adafruit.com/datasheets/785060-2500mAh_specification_sheet.pdf
 * Discharge cutoff at 2.75V
 */
#include <Arduino.h>

#include "debug.h"
#include "pins.h"

// Turn warning light on at 3.5V
const int batteryVoltageLow = 716; // 3.5 / 5 * 1023
int batteryVoltage = 0;
bool batteryStateLowDetected = false;

// Set up battery monitoring
void setupBattery() {
    pinMode(PIN_BATTERY, INPUT);
    pinMode(PIN_BATTERY_STATUS, OUTPUT);
}

// Monitor the battery
void monitorBattery() {
    if (batteryStateLowDetected) {
        return NULL;
    }

    batteryVoltage = analogRead(PIN_BATTERY);

    DPRINTF("Voltage: ");
    DPRINTLN(batteryVoltage);

    if (batteryVoltage <= batteryVoltageLow) {
        digitalWrite(PIN_BATTERY_STATUS, HIGH);
        batteryStateLowDetected = true;
    }
}
