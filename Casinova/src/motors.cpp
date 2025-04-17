#include <Arduino.h>
#include "motors.h"

// Swivel Motor
#define stepPin 1
#define dirPin  2
#define STEPS_PER_REV 200
#define MICROSTEPPING 1
#define GEAR_RATIO 1
#define TOTAL_STEPS (STEPS_PER_REV * MICROSTEPPING * GEAR_RATIO)
int currentPosition = 0;


// Ejection Motor
#define ejectEn  42  // Replace with your actual enable pin
#define ejectIn1 38
#define ejectIn2 39


void setupSwivel() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
}

void rotateToPlayer(int playerIndex, int totalPlayers) {
    float anglePerPlayer = 360.0 / totalPlayers;
    float targetAngle = anglePerPlayer * playerIndex;

    int targetStep = (int)((targetAngle / 360.0) * TOTAL_STEPS);

    int stepDelta = targetStep - currentPosition;

    // Constrains rotation to the shortest path
    if (stepDelta > TOTAL_STEPS / 2) {
        stepDelta -= TOTAL_STEPS;
    } else if (stepDelta < -TOTAL_STEPS / 2) {
        stepDelta += TOTAL_STEPS;
    }

    int stepDir = (stepDelta >= 0) ? HIGH : LOW;
    digitalWrite(dirPin, stepDir);

    int steps = abs(stepDelta);
    for (int i = 0; i < steps; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);  // Tune for motor speed
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
    }

    // Tracks current position with wrap-around
    currentPosition = (currentPosition + stepDelta + TOTAL_STEPS) % TOTAL_STEPS;
}




void setupEjection() {
    // pinMode(ejectEn, OUTPUT);
    // pinMode(ejectIn1, OUTPUT);
    // pinMode(ejectIn2, OUTPUT);

    // digitalWrite(ejectIn1, HIGH);  // forward direction
    // digitalWrite(ejectIn2, LOW);
    // analogWrite(ejectEn, 0);       // off initially

    pinMode(ejectEn, OUTPUT);
    digitalWrite(ejectEn, LOW);
}

void runEjection(int distance_mm) {
    // // Clamp distance between 0 and 2000 mm
    // distance_mm = constrain(distance_mm, 0, 2000);

    // // Map to PWM range (adjust as needed)
    // int motorPower = map(distance_mm, 0, 2000, 50, 200);  // out of 255

    // Serial.printf("Ejection motor: distance=%dmm, speed=%d\n", distance_mm, motorPower);

    // analogWrite(ejectEn, motorPower);


    int motorPower = map(constrain(distance_mm, 0, 2000), 0, 2000, 50, 255);
    int duration_ms = 1000; // run for 1s while testing

    Serial.printf("Ejection (LED) at power %d\n", motorPower);
    analogWrite(ejectEn, motorPower);
    delay(duration_ms);
    analogWrite(ejectEn, 0);
}

