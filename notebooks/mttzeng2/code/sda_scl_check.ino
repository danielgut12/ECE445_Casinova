#include <Arduino.h>
#include <Wire.h>

#define SDA_PIN 8  // Change this to your SCCB SDA
#define SCL_PIN 9  // Change this to your SCCB SCL

void setup() {
    Serial.begin(115200);
    pinMode(SDA_PIN, INPUT_PULLUP);
    pinMode(SCL_PIN, INPUT_PULLUP);
    Wire.begin(SDA_PIN, SCL_PIN, 50000); // Reduce I2C speed to 50kHz

    Serial.println("Scanning for I2C devices...");
    for (uint8_t address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            Serial.printf("Device found at 0x%X\n", address);
        }
    }
    Serial.println("Scan complete.");
}

void loop() {
    delay(5000);
}