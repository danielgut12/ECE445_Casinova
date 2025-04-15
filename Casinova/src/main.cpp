#include <Arduino.h>
#include "card.h"
#include "motors.h"
#include "tof_sensor.h"
#include "input.h"
#include "camera.h"
#include "my_wifi.h"
#include "mjpeg_streamer.h"

// MJPEGStreamer *streamer;

void setup() {
  Serial.begin(115200);
  // initStepper();
  // initTOFSensor();
  // initMotors();
  // initInputs();
  initTOFSensor();
  // init_camera();
  initWifi();

  // static MJPEGStreamer mjpeg(server);
  // streamer = &mjpeg;
  // streamer->begin();


  // visual check
  pinMode(42, OUTPUT);
  digitalWrite(42, HIGH);
  delay(100);
}

void loop() {

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