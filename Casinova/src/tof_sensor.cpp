#include <Arduino.h>
#include "tof_sensor.h"

#define RXD2 39   // TOF10120 TX (White) → ESP32 RX
#define TXD2 38   // TOF10120 RX (Yellow) → ESP32 TX

static String distanceBuffer = "";  // Internal buffer

void initTOFSensor() {
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Serial.println("TOF10120 Sensor Initialized.");
}

float getPlayerDistance() {
    while (Serial2.available()) {
        char c = Serial2.read();

        if (c == '\n' || c == '\r') {
            if (distanceBuffer.length() > 0) {
                float dist = distanceBuffer.toFloat();
                distanceBuffer = "";
                return dist;
            }
        } else if (isDigit(c)) {
            distanceBuffer += c;
        }
    }

    return -1.0; // No new valid reading yet
}
