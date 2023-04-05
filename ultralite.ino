#include "debug.h"
#include "blower.h"

// ##################################
// Setup
// ##################################
void setup() {
    #ifdef DEBUG
    Serial.begin(115200);
    #endif

    setupBlower();

    DPRINTLN("Setup complete");
}

// ##################################
// Loop
// ##################################
void loop() {
    processProgramButtonPress();
    runDefaultProgram();
}
