#include "debug.h"
#include "blower.h"
#include "battery.h"

// ##################################
// Setup
// ##################################
void setup() {
    #ifdef DEBUG
    Serial.begin(115200);
    #endif

    setupBlower();
    setupBattery();

    DPRINTLN("Setup complete");

}

// ##################################
// Loop
// ##################################
void loop() {
    processProgramButtonPress();
    runDefaultProgram();
    monitorBattery();
}
