#include <Arduino.h>
#include "motors.h"
#include "tof_sensor.h"

// Swivel Motor
#define dirPin  20
#define stepPin 21
#define enablePin 2 // DON'T USE GPIO1 ITS FOR SERIAL STUFF AND IT WONT WORK
#define STEPS_PER_REV 200
#define MICROSTEPPING 1
#define GEAR_RATIO 1
#define TOTAL_STEPS (STEPS_PER_REV * MICROSTEPPING * GEAR_RATIO)
int currentPosition = 0;


// Ejection Motor
#define ejectPin    42
#define swivelPin   43
#define queuePin    44


void initMotors() {
    pinMode(enablePin, OUTPUT); // set pin as output
    digitalWrite(enablePin, HIGH); // DISABLE motor
    Serial.println("fuck");
}

void setupSwivel() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
}

void rotateStepper(int degrees) {
    int steps = (abs(degrees) * TOTAL_STEPS) / 360;

    digitalWrite(dirPin, degrees > 0 ? HIGH : LOW);

    int startDelay = 12000;  // Super slow start
    int endDelay = 3000;     // Slow cruising
    int stepDelay = startDelay;
    int accelSteps = steps * 3 / 4;
    int decelStart = steps - accelSteps;

    for (int i = 0; i < steps; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(stepDelay);

        // Super gentle ramp
        if (i < accelSteps && stepDelay > endDelay) {
            if (i % 2 == 0) stepDelay -= 1;
        }
        if (i > decelStart && stepDelay < startDelay) {
            if (i % 2 == 0) stepDelay += 1;
        }
    }

    Serial.print("Rotated ");
    Serial.print(degrees);
    Serial.println(" degrees (ultra smooth).");
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

void rotateByDegrees(float degrees) {
    int steps = (int)((degrees / 360.0) * TOTAL_STEPS);
    if (steps == 0) return;

    int stepDir = (steps >= 0) ? HIGH : LOW;
    digitalWrite(dirPin, stepDir);

    for (int i = 0; i < abs(steps); i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);  // Tune as needed
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
    }

    // Update current position with wrap-around
    currentPosition = (currentPosition + steps + TOTAL_STEPS) % TOTAL_STEPS;
}

void setupEjection() {
    pinMode(ejectPin, OUTPUT);
    digitalWrite(ejectPin, LOW);
}

void runEjection(int distance_mm) {
    // // Clamp distance between 0 and 2000 mm
    // distance_mm = constrain(distance_mm, 0, 2000);

    // // Map to PWM range (adjust as needed)
    // int motorPower = map(distance_mm, 0, 2000, 50, 200);  // out of 255

    
    // analogWrite(ejectEn, motorPower);
    
    // Maps 0mm to 50 power and 2000mm to 255 power
    int motorPower = map(constrain(distance_mm, 0, 2000), 0, 2000, 50, 255);
    int duration_ms = 1000; // run for 1s while testing
    
    Serial.printf("Ejection (LED) at power %d\n", motorPower);
    analogWrite(ejectPin, motorPower);
    delay(duration_ms);
    analogWrite(ejectPin, 0);
    Serial.printf("Ejection motor: distance=%dmm, speed=%d\n", distance_mm, motorPower);
}


void pushCardsWithDistance() {
    float distance = getPlayerDistance();
    Serial.printf("Distance: %.2f mm (%.2f in)\n", distance, distance / 25.4);
    
    if (distance >= 0.0) {
        runEjection(distance);
    } else {
        Serial.println("No valid TOF reading, skipping ejection.");
    }
}

void queueCard() {
    // Enable the driver
    digitalWrite(enablePin, LOW); // ENABLE active LOW

    // Set motor direction (clockwise)
    digitalWrite(dirPin, LOW);

    // Acceleration setup
    int startDelay = 5000; // Start slow (bigger delay = slower)
    int endDelay = 800;    // Final speed (your 8000us originally divided by 10)
    int stepDelay = startDelay;

    // Step the motor one full revolution
    for (int i = 0; i < STEPS_PER_REV; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(stepDelay);

        // Accelerate: gradually reduce stepDelay
        if (stepDelay > endDelay) {
            stepDelay -= 10; // Decrease delay slowly each step
        }
    }

    // Disable the driver after movement
    digitalWrite(enablePin, HIGH); // DISABLE motor

    Serial.println("Card queued.");
}





// Rotates to board location (unknown atm does nothing)
void rotateToBoard() {
    return;
}