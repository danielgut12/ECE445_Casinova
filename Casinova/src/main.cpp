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

  // pinMode(stepPin, OUTPUT);
  // pinMode(dirPin, OUTPUT);
  // initStepper();
  // initTOFSensor();
  initMotors();
  // initInputs();
  // init_camera(); // TURN BACK ON FOR CAMERA
  initWifi(); // TURN BACK ON FOR WIFI

  // initMockDeck(); // Test deck without card detection


  // pinMode(stepPin, OUTPUT);
  // pinMode(dirPin, OUTPUT);
  // pinMode(enablePin, OUTPUT);

  // digitalWrite(enablePin, LOW); // Enable motor driver


  // visual check
  // pinMode(42, OUTPUT);
  // digitalWrite(42, HIGH);
  delay(100);
}

void loop() {

  // for (int i = 0; i < 4; i++) {
  //     makeQuarterTurn();
  //     delay(1000);
  //     ejectCard(170);
  // }
  
  // resetToStart();

  // delay(500);

}

