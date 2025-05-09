#include <Arduino.h>
#include "esp_system.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Checking Flash & PSRAM...");

    // Check Flash Size
    Serial.printf("Flash Chip Size: %dMB\n", spi_flash_get_chip_size() / (1024 * 1024));

    // Check if PSRAM is available
    if (psramInit()) {
        Serial.printf("PSRAM Available: %dMB\n", ESP.getPsramSize() / (1024 * 1024));
    } else {
        Serial.println("PSRAM NOT AVAILABLE!");
    }
}

void loop() {
    // Nothing to do in loop
}