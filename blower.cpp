#include <Arduino.h>
#include <TimerOne.h>
#include <string.h>

#include "debug.h"
#include "pins.h"

// Valid program states
enum programStates {
    PROGRAM_UNSET,
    PROGRAM_DEFAULT,
    PROGRAM_1,
    PROGRAM_2,
    PROGRAM_3,
    PROGRAM_4,
    PROGRAM_OFF
};

// Variables for debouncing the program button
int programRead;
int programPrev = HIGH;
unsigned long programPrevTime = 0;
const unsigned long programDebounceDelay = 50;
programStates programState = PROGRAM_UNSET;

// Set up default program
unsigned long defaultProgramPrevTime = 0;
unsigned long defaultProgramInterval = 1000;
float defaultProgramStep = 0;
uint8_t defaultProgramMaxStep = 60;

// Set up blower
void setupBlower() {
    Timer1.initialize(40);

    pinMode(PIN_PROGRAM, INPUT_PULLUP);
    pinMode(PIN_STATUS, OUTPUT);
    pinMode(PIN_BLOWER, OUTPUT);
}

// Set blower PWM
void setBlower(float dutyCycle, bool statusLed) {
    digitalWrite(PIN_STATUS, statusLed ? HIGH : LOW);
    Timer1.pwm(PIN_BLOWER, dutyCycle / 100 * 1023);
}


// Set current progamm state
void setProgram(programStates state) {
    programState = state;

    // Set blower speed
    if (programState == PROGRAM_DEFAULT) {
        DPRINTLN("Program: Default");
    } else if (programState == PROGRAM_1) {
        DPRINTLN("Program: 1");
        setBlower(25, true);
    } else if (programState == PROGRAM_2) {
        DPRINTLN("Program: 2");
        setBlower(50, true);
    } else if (programState == PROGRAM_3) {
        DPRINTLN("Program: 3");
        setBlower(75, true);
    } else if (programState == PROGRAM_4) {
        DPRINTLN("Program: 4");
        setBlower(100, true);
    } else if (programState == PROGRAM_OFF) {
        DPRINTLN("Program: Off");
        setBlower(0, false);
    }
}

// Set next progamm state
void setNextProgramState() {
    if (programState == PROGRAM_UNSET) {
        setProgram(PROGRAM_DEFAULT);
    } else if (programState == PROGRAM_DEFAULT || programState == PROGRAM_OFF) {
        setProgram(PROGRAM_1);
    } else if (programState == PROGRAM_1) {
        setProgram(PROGRAM_2);
    } else if (programState == PROGRAM_2) {
        setProgram(PROGRAM_3);
    } else if (programState == PROGRAM_3) {
        setProgram(PROGRAM_4);
    } else if (programState == PROGRAM_4) {
        setProgram(PROGRAM_OFF);
    }
}

// Ramp up the blower speed slowly over a given period
void runDefaultProgram() {
    if (programState != PROGRAM_DEFAULT) {
        return NULL;
    }

    if (millis() - defaultProgramPrevTime > defaultProgramInterval) {
        defaultProgramPrevTime = millis();
        defaultProgramStep++;
    } else {
        return NULL;
    }

    if (defaultProgramStep >= defaultProgramMaxStep) {
        DPRINTLN("Default program complete");
        setProgram(PROGRAM_4);
        return NULL;
    }

    setBlower(100 / defaultProgramMaxStep * defaultProgramStep, true);
}

// Read program button press and set program accordingly
void processProgramButtonPress() {
    programRead = digitalRead(PIN_PROGRAM);

    // Debounce program button
    if (programRead == programPrev || millis() - programPrevTime < programDebounceDelay) {
        return NULL;
    }

    // Set program state
    programPrev = programRead;
    programPrevTime = millis();
    if (programRead == LOW) {
        setNextProgramState();
    }
}
