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



#define EJECTION_PIN_INPUT1 48
#define EJECTION_PIN_INPUT2 21
#define EJECTION_PWM_PIN 47

#define SUPPLY_STEP_PIN 39
#define SUPPLY_DIRECTION_PIN 38
#define STEPS_PER_REVOLUTION 200

#define SWIVEL_STEP_PIN 5
#define SWIVEL_DIRECTION_PIN 4





void setupEjection() {
    pinMode(EJECTION_PIN_INPUT1, OUTPUT);
    pinMode(EJECTION_PIN_INPUT2, OUTPUT);
    pinMode(EJECTION_PWM_PIN, OUTPUT);
}

void setupSwivel() {
    pinMode(SWIVEL_STEP_PIN, OUTPUT);
    pinMode(SWIVEL_DIRECTION_PIN, OUTPUT);
}

void setupSupply() {
    pinMode(SUPPLY_STEP_PIN, OUTPUT);
    pinMode(SUPPLY_DIRECTION_PIN, OUTPUT);
}
void initMotors() {
    setupEjection();
    setupSwivel();
    setupSupply();
}

void ejectCard(int power) {
    analogWrite(EJECTION_PWM_PIN, 120); //ENA  pin
  
    // turn on ejection motor
    digitalWrite(EJECTION_PIN_INPUT1,  LOW);
    digitalWrite(EJECTION_PIN_INPUT2, HIGH);
    delay(1000);
  
    // turn on supply motor
    digitalWrite(SUPPLY_DIRECTION_PIN, HIGH);
  
    // Spin motor quickly
    for(int x = 0; x < 4 * (STEPS_PER_REVOLUTION / 10); x++)
    {
      digitalWrite(SUPPLY_STEP_PIN, HIGH);
      delayMicroseconds(4000);
      digitalWrite(SUPPLY_STEP_PIN, LOW);
      delayMicroseconds(4000);
    }
    delay(500);
  
    // turn off ejection motor
    digitalWrite(EJECTION_PIN_INPUT1, LOW);
    digitalWrite(EJECTION_PIN_INPUT2, LOW);
    delay(1000);




    analogWrite(EJECTION_PWM_PIN, 255); //ENA  pin
  
    // turn on ejection motor
    digitalWrite(EJECTION_PIN_INPUT1,  LOW);
    digitalWrite(EJECTION_PIN_INPUT2, HIGH);
    delay(1000);
  
    // turn on supply motor
    digitalWrite(SUPPLY_DIRECTION_PIN, HIGH);
  
    // Spin motor quickly
    for(int x = 0; x < 4 * (STEPS_PER_REVOLUTION / 10); x++)
    {
      digitalWrite(SUPPLY_STEP_PIN, HIGH);
      delayMicroseconds(4000);
      digitalWrite(SUPPLY_STEP_PIN, LOW);
      delayMicroseconds(4000);
    }
    delay(500);
  
    // turn off ejection motor
    digitalWrite(EJECTION_PIN_INPUT1, LOW);
    digitalWrite(EJECTION_PIN_INPUT2, LOW);
    delay(1000);
  }

  void makeQuarterTurnCW() {
    digitalWrite(SWIVEL_DIRECTION_PIN, LOW);
  
    // Spin motor acceleration
    for (int x = 0; x < 10; x++) {
        digitalWrite(SWIVEL_STEP_PIN, HIGH);
        delayMicroseconds(14000);
        digitalWrite(SWIVEL_STEP_PIN, LOW);
        delayMicroseconds(14000);
    }
  
    // Spin motor
    for (int x = 0; x < 30; x++) {
        digitalWrite(SWIVEL_STEP_PIN, HIGH);
        delayMicroseconds(12000);
        digitalWrite(SWIVEL_STEP_PIN, LOW);
        delayMicroseconds(12000);
    }

    // Spin motor deceleration
    for (int x = 0; x < 20; x++) {
        digitalWrite(SWIVEL_STEP_PIN, HIGH);
        delayMicroseconds(14000);
        digitalWrite(SWIVEL_STEP_PIN, LOW);
        delayMicroseconds(14000);
    }

    // Stopping the jiggle
    digitalWrite(SWIVEL_DIRECTION_PIN, HIGH);
    for (int x = 0; x < 2; x++) {
        digitalWrite(SWIVEL_STEP_PIN, HIGH);
        delayMicroseconds(14000);
        digitalWrite(SWIVEL_STEP_PIN, LOW);
        delayMicroseconds(14000);
    }
    digitalWrite(SWIVEL_DIRECTION_PIN, LOW);

    
}

void makeQuarterTurnCCW() {
    digitalWrite(SWIVEL_DIRECTION_PIN, HIGH);
  
    // Spin motor acceleration
    for (int x = 0; x < 10; x++) {
        digitalWrite(SWIVEL_STEP_PIN, HIGH);
        delayMicroseconds(14000);
        digitalWrite(SWIVEL_STEP_PIN, LOW);
        delayMicroseconds(14000);
    }
  
    // Spin motor
    for (int x = 0; x < 30; x++) {
        digitalWrite(SWIVEL_STEP_PIN, HIGH);
        delayMicroseconds(12000);
        digitalWrite(SWIVEL_STEP_PIN, LOW);
        delayMicroseconds(12000);
    }

    // Spin motor deceleration
    for (int x = 0; x < 20; x++) {
        digitalWrite(SWIVEL_STEP_PIN, HIGH);
        delayMicroseconds(14000);
        digitalWrite(SWIVEL_STEP_PIN, LOW);
        delayMicroseconds(14000);
    }

    // Stopping the jiggle
    digitalWrite(SWIVEL_DIRECTION_PIN, HIGH);
    for (int x = 0; x < 2; x++) {
        digitalWrite(SWIVEL_STEP_PIN, HIGH);
        delayMicroseconds(14000);
        digitalWrite(SWIVEL_STEP_PIN, LOW);
        delayMicroseconds(14000);
    }
    digitalWrite(SWIVEL_DIRECTION_PIN, HIGH);
}

void resetToStart() {
    digitalWrite(SWIVEL_DIRECTION_PIN, HIGH);
    for (int x = 0; x < STEPS_PER_REVOLUTION; x++) {
        digitalWrite(SWIVEL_STEP_PIN, HIGH);
        delayMicroseconds(12000);
        digitalWrite(SWIVEL_STEP_PIN, LOW);
        delayMicroseconds(12000);
    }
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