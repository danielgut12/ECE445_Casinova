#include <Arduino.h>
#include "tof_sensor.h"

#define RXD2 41   // TOF10120 TX (White) → ESP32 RX
#define TXD2 40   // TOF10120 RX (Yellow) → ESP32 TX

void initTOFSensor() {
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Serial.println("TOF10120 Sensor Initialized.");
}


// --- Global ---
static String distanceBuffer = "";
static float latestDistance = -1.0;

// --- Call this often (inside loop!) ---
void updatePlayerDistance() {
    while (Serial2.available()) {
        char c = Serial2.read();

        if (c == '\n' || c == '\r') {
            if (distanceBuffer.length() > 0) {
                latestDistance = distanceBuffer.toFloat();
                distanceBuffer = "";
            }
        }
        else if (isDigit(c) || c == '.' || c == '-') {
            distanceBuffer += c;
        }
    }
}

// --- Call this whenever you want the latest reading ---
float getPlayerDistance() {
    return latestDistance;
}
