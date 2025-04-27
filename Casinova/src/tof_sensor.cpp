#include <Arduino.h>
#include "tof_sensor.h"

#define RXD2 41   // TOF10120 TX (White) → ESP32 RX
#define TXD2 40   // TOF10120 RX (Yellow) → ESP32 TX

static String distanceBuffer = "";  // Internal buffer

void initTOFSensor() {
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Serial.println("TOF10120 Sensor Initialized.");
}

float getPlayerDistance() {
    String distanceBuffer = "";

    // Wait until a full number is received (with timeout safety)
    unsigned long startTime = millis();
    while (millis() - startTime < 100) { // wait max 100 ms
        while (Serial2.available()) {
            char c = Serial2.read();

            if (c == '\n' || c == '\r') {
                if (distanceBuffer.length() > 0) {
                    return distanceBuffer.toFloat();
                }
            }
            else if (isDigit(c) || c == '.' || c == '-') {
                distanceBuffer += c;
            }
            // else: ignore weird chars
        }
    }

    return -1.0; // No complete number after timeout
}
