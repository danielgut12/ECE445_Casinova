#include <Arduino.h>

#define RXD2 42   // TOF10120 TX (White) → ESP32 RX
#define TXD2 41  // TOF10120 RX (Yellow) → ESP32 TX

String distance = "";  // Buffer to store the full reading

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Serial.println("TOF10120 Real-Time Distance Readings...");
}

void loop() {
    while (Serial2.available()) {
        char c = Serial2.read();

        if (c == '\n' || c == '\r') {  
            // Newline or carriage return means the number is complete
            if (distance.length() > 0) {
                Serial.print("Distance: ");
                Serial.print(distance);
                Serial.println(" mm");
                distance = "";  // Reset buffer for next reading
            }
        } else if (isDigit(c)) {  
            // Append only numeric characters to build the distance value
            distance += c;
        }
    }
}

