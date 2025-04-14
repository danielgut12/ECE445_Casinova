#include <Arduino.h>
#include "card.h"
#include "motors.h"
#include "tof_sensor.h"
#include "input.h"
#include "camera.h"

void setup() {
  Serial.begin(115200);
  // initStepper();
  // initTOFSensor();
  // initMotors();
  // initInputs();
  initTOFSensor();
  // init_camera();

  pinMode(42, OUTPUT);
  digitalWrite(42, HIGH);
  delay(100);
}

void loop() {
  // capture_and_send();
  float dist_mm = getPlayerDistance();
  if(dist_mm > 0) Serial.println(dist_mm);
    // delay(1000);
  // int playerIndex;
  // if (checkHitInput(playerIndex)) {
  //   handleHit(playerIndex);
  // }
  // Add check for stand, new round, etc.
}

// void handleHit(int playerIndex) {
//   rotateToPlayer(playerIndex);

//   Card dealtCard;
//   captureAndDetectCard(dealtCard);

//   pushCardToEjectionZone();

//   float distance = getPlayerDistance();
//   ejectCardToDistance(distance);

//   storeDealtCard(playerIndex, dealtCard); // Optional for tracking
// } 