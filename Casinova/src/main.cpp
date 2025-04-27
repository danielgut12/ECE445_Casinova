#include <Arduino.h>
#include "card.h"
#include "motors.h"
#include "tof_sensor.h"
#include "input.h"
#include "camera.h"
#include "my_wifi.h"
#include "structs.h"
#include "deck.h"
#include "game.h"


// const int dirPin = 20;
// const int stepPin = 21;
// const int stepsPerRevolution = 200;

#define dirPin  20
#define stepPin 21
#define enablePin 2 // DON'T USE GPIO1 ITS FOR SERIAL STUFF AND IT WONT WORK

#define STEPS_PER_REV 200  // Standard 1.8° stepper motor
#define MICROSTEPPING 1    // 1 if no microstepping
#define GEAR_RATIO 1       // 1 if no gear reduction
#define TOTAL_STEPS (STEPS_PER_REV * MICROSTEPPING * GEAR_RATIO)

void setup() {
  
  Serial.begin(115200);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  // initStepper();
  initTOFSensor();
  initMotors();
  // initInputs();
  // init_camera(); // TURN BACK ON FOR CAMERA
  initWifi(); // TURN BACK ON FOR WIFI

  initMockDeck(); // Test deck without card detection


  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  digitalWrite(enablePin, LOW); // Enable motor driver


  // visual check
  // pinMode(42, OUTPUT);
  // digitalWrite(42, HIGH);
  delay(100);
}

void loop() {
  updatePlayerDistance(); // Keep this on so it constantly refreshes
  broadcastDistance();

  // rotateStepper(180);   // Rotate 180 degrees forward
  // delay(5000);          // Pause
  // rotateStepper(-180);  // Rotate 180 degrees backward
  // delay(5000);          // Pause
    // Set motor direction clockwise
    digitalWrite(dirPin, LOW);

    digitalWrite(stepPin, HIGH);
    delayMicroseconds(3000); // Pulse HIGH for 1000 microseconds
    digitalWrite(stepPin, LOW);
    delayMicroseconds(3000); // Pulse LOW for 1000 microseconds
}
