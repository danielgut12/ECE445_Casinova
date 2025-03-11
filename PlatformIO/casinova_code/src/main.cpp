#include <Arduino.h>

#define RXD2 42   // TOF10120 TX (White) → ESP32 RX
#define TXD2 41  // TOF10120 RX (Yellow) → ESP32 TX

String distance = "";  // Buffer to store the full reading
float dist_flt = 0.0;
int motor_power = 0;

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
                // Serial.print("Distance: ");
                // Serial.print(distance);
                // Serial.println(" mm");
                dist_flt = atof(distance.c_str()); // Convert to float
                motor_power = map(dist_flt, 300, 1800, 50, 255);
                Serial.println(motor_power);
                



                distance = "";  // Reset buffer for next reading
            }
        } else if (isDigit(c)) {  
            // Append only numeric characters to build the distance value
            distance += c;

            Serial.print(dist_flt);
        }
    }
}



// #include <Arduino.h>
// #include "esp_camera.h"

// // #define XCLK_PIN 21  // GPIO for XCLK (MCLK)

// // AVOID 6-11  (Quad SPI Flash)
// // AVOID 26-33 (Octal PSRAM)
// // GPIO Pin Mapping (Use your setup)
// #define PWDN_GPIO_NUM    -1
// #define RESET_GPIO_NUM   -1
// // #define XCLK_GPIO_NUM     -1
// #define SIOD_GPIO_NUM     8 // SDA
// #define SIOC_GPIO_NUM     9 // SCL

// #define Y7_GPIO_NUM       18
// #define Y6_GPIO_NUM       17
// #define Y5_GPIO_NUM       16
// #define Y4_GPIO_NUM       15
// #define Y3_GPIO_NUM        5
// #define Y2_GPIO_NUM        4
// #define Y1_GPIO_NUM        2
// #define Y0_GPIO_NUM        1
// #define VSYNC_GPIO_NUM    13
// #define HREF_GPIO_NUM     12
// #define PCLK_GPIO_NUM     46

// void resetCamera() {
//   pinMode(PWDN_GPIO_NUM, OUTPUT);
//   digitalWrite(PWDN_GPIO_NUM, LOW);  // Ensure camera is powered ON

//   pinMode(RESET_GPIO_NUM, OUTPUT);
//   digitalWrite(RESET_GPIO_NUM, LOW);  // Hold in reset
//   delay(10);
//   digitalWrite(RESET_GPIO_NUM, HIGH); // Release reset
//   delay(10);
// }


// void setup() {
//     Serial.begin(115200);
//     Serial.println("Initializing OV2640...");
  
//     // Config Camera
//     camera_config_t config;
//     config.ledc_channel = LEDC_CHANNEL_0;
//     config.ledc_timer = LEDC_TIMER_0;
//     config.pin_d0 = Y0_GPIO_NUM;
//     config.pin_d1 = Y1_GPIO_NUM;
//     config.pin_d2 = Y2_GPIO_NUM;
//     config.pin_d3 = Y3_GPIO_NUM;
//     config.pin_d4 = Y4_GPIO_NUM;
//     config.pin_d5 = Y5_GPIO_NUM;
//     config.pin_d6 = Y6_GPIO_NUM;
//     config.pin_d7 = Y7_GPIO_NUM;
//     config.pin_xclk = -1; // External Clock being used
//     config.pin_pclk = PCLK_GPIO_NUM;
//     config.pin_vsync = VSYNC_GPIO_NUM;
//     config.pin_href = HREF_GPIO_NUM;
//     config.pin_sccb_sda = SIOD_GPIO_NUM;
//     config.pin_sccb_scl = SIOC_GPIO_NUM;
//     config.pin_pwdn = PWDN_GPIO_NUM;
//     config.pin_reset = RESET_GPIO_NUM;
//     config.xclk_freq_hz = 24000000;  // Set XCLK to 24MHz
//     config.pixel_format = PIXFORMAT_GRAYSCALE;  // Grayscale lowest size
//     config.frame_size = FRAMESIZE_QQVGA;  // Use lowest resolution (160x120)
//     config.fb_location = CAMERA_FB_IN_PSRAM; // Store the frame buffer in the internal RAM
//     config.fb_count = 1;
//     config.jpeg_quality = 30;  // Adjust for quality (lower = better quality)

//     resetCamera();
//     esp_err_t err = esp_camera_init(&config);
//     if (err != ESP_OK) {
//         Serial.printf("Camera init failed! Error 0x%x\n", err);
//         return;
//     }
//     Serial.println("Camera Ready!");
// }

// void loop() {
//     Serial.println("Capturing Image...");
//     camera_fb_t *fb = esp_camera_fb_get();
//     if (!fb) {
//         Serial.println("Camera capture failed");
//         delay(1000);
//         return;
//     }
//     Serial.printf("Captured Image: %d bytes\n", fb->len);
//     for (size_t i = 0; i < fb->len; i++) {
//         Serial.printf("%02X ", fb->buf[i]);
//         if ((i + 1) % 32 == 0) Serial.println();
//     }
//     Serial.println();
//     esp_camera_fb_return(fb);
//     delay(1000000);
// }
