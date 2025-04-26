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

// MJPEGStreamer *streamer;

const int dirPin = 1;
const int stepPin = 2;
const int stepsPerRevolution = 200;


void setup() {
  
  Serial.begin(115200);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  // initStepper();
  // initTOFSensor();
  // initMotors();
  // initInputs();
  // initTOFSensor();
  init_camera(); // TURN BACK ON FOR CAMERA
  initWifi(); // TURN BACK ON FOR WIFI

  initMockDeck(); // Test deck without card detection

  // visual check
  pinMode(42, OUTPUT);
  digitalWrite(42, HIGH);
  delay(100);
}

void loop() {
  // capture_and_send();
  // delay(100000);
    // // Set motor direction clockwise
    // digitalWrite(dirPin, HIGH);
  
    // // Spin motor slowly
    // for (int i = 0; i < 8; i++) {
    //   for(int x = 0; x < stepsPerRevolution / 8; x++)
    //   {
    //     digitalWrite(stepPin, HIGH);
    //     delayMicroseconds(2000);
    //     digitalWrite(stepPin, LOW);
    //     delayMicroseconds(2000);
    //   }
    //   delay(1000); // Wait a second
    // }
    
    // // Set motor direction counterclockwise
    // digitalWrite(dirPin, LOW);
  
    // // Spin motor quickly
    // for(int x = 0; x < stepsPerRevolution; x++)
    // {
    //   digitalWrite(stepPin, HIGH);
    //   delayMicroseconds(2000);
    //   digitalWrite(stepPin, LOW);
    //   delayMicroseconds(2000);
    // }
    // delay(1000); // Wait a second
  




  // capture_and_send();
  // delay(100000);
  // float dist_mm = getPlayerDistance();
  // if(dist_mm > 0) Serial.println(dist_mm);
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