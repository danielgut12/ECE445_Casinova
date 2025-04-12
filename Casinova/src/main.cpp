#include <Arduino.h>
#include "card.h"
#include "motors.h"
#include "tof_sensor.h"
#include "input.h"
#include "camera.h"

void setup() {
  Serial.begin(115200);
  initStepper();
  initCamera();
  initTOFSensor();
  initMotors();
  initInputs();
}

void loop() {
  int playerIndex;
  if (checkHitInput(playerIndex)) {
    handleHit(playerIndex);
  }
  // Add check for stand, new round, etc.
}

void handleHit(int playerIndex) {
  rotateToPlayer(playerIndex);

  Card dealtCard;
  captureAndDetectCard(dealtCard);

  pushCardToEjectionZone();

  float distance = getPlayerDistance();
  ejectCardToDistance(distance);

  storeDealtCard(playerIndex, dealtCard); // Optional for tracking
} 