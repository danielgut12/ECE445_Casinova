#include <Arduino.h>
#include <WiFiMulti.h>
#include <WebServer.h>

#define WIFI_SSID "2.4 410GRN-U313"
#define WIFI_PASSWORD "CXNK001CBEAB"

WiFiMulti wifiMulti;
WebServer server(80); // Port 80 for HTTP
String encoded = "initializing";

void handleRoot() {
    server.send(200, "text/plain", encoded);
}
// OV2640 CAMERA STUFF
/*
#include "esp_camera.h"
#include "base64.h"
#include "esp_log.h"

// WROVER-KIT PIN Map
#define CAM_PIN_PWDN    -1
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK    12  // Verified works at 12 with oscilliscope
#define CAM_PIN_SIOD    13   // data transfer good at 13
#define CAM_PIN_SIOC    14  // data transfer good at 14

#define CAM_PIN_D7      18
#define CAM_PIN_D6      17
#define CAM_PIN_D5      16
#define CAM_PIN_D4      15
#define CAM_PIN_D3       7
#define CAM_PIN_D2       6
#define CAM_PIN_D1       5
#define CAM_PIN_D0       4
#define CAM_PIN_VSYNC   10
#define CAM_PIN_HREF     9
#define CAM_PIN_PCLK    11  // Verified works at 11 with oscilliscope

static camera_config_t camera_config = {
    .pin_pwdn  = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,
    
    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,
    
    .xclk_freq_hz = 12000000, 
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    
    .pixel_format = PIXFORMAT_RGB565,
    .frame_size = FRAMESIZE_QVGA,       // 320x240
    .jpeg_quality = 12,                 // 0 = highest, 63 = worst
    .fb_count = 2,                      // JPEG needs double buffers
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY     // Recommended for JPEG
};


void process_image(int width, int height, pixformat_t format, uint8_t *buf, size_t len) {
    Serial.printf("Image captured: %dx%d, format=%d, size=%d bytes\n", width, height, format, len);
    encoded = base64::encode(buf, len);
    // Serial.println(encoded); // Base64 encoded RGB565
}

esp_err_t camera_init() {
    if (CAM_PIN_PWDN != -1) {
        pinMode(CAM_PIN_PWDN, OUTPUT);
        digitalWrite(CAM_PIN_PWDN, LOW);
        delay(10); // Give it time to power up
    }
    
    delay(100); // Give time before init
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        Serial.println("Camera Init Failed");
    }
    return err;
}


esp_err_t camera_capture() {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera Capture Failed: framebuffer is NULL");
        return ESP_FAIL;
    }
    
    Serial.printf("Captured JPEG: %dx%d, %d bytes\n", fb->width, fb->height, fb->len);
    process_image(fb->width, fb->height, fb->format, fb->buf, fb->len);
    esp_camera_fb_return(fb);
    return ESP_OK;
}

*/



void setup() {
    Serial.begin(115200);
    // digitalWrite(LED_BUILTIN, 0);

    delay(1000); // Allow time for serial monitor

    
    // esp_log_level_set("*", ESP_LOG_VERBOSE);
    
    // if (!psramFound()) {
    //     Serial.println("PSRAM not found!");
    // }
    
    // WIFI STUFF
    pinMode(LED_BUILTIN, OUTPUT);
    
    // addAP req. two args, (SSID, PASSWORD)
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

    // Keeps trying to connect to wifi until WL_CONNECTED signal given
    while(wifiMulti.run() != WL_CONNECTED) {
        delay(100);
    }
    
    // Serial Monitor Logging
    Serial.println("Connected to Wifi");
    Serial.println("Connected to Wifi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    
    server.on("/", handleRoot); // Define route
    server.begin();
    
    Serial.print("HTTP server started");


    // if (camera_init() == ESP_OK) {
    //     Serial.println("Camera initialized successfully");
    // } else {
    //     Serial.println("Camera initialization failed");
    // }
    // Serial.printf("PSRAM: %s, size: %d bytes\n", psramFound() ? "OK" : "NOT FOUND", ESP.getPsramSize());
    // sensor_t * s = esp_camera_sensor_get();
    // if (!s) {
    //     Serial.println("Failed to get sensor object");
    // }

    // /*
    // s->set_framesize(s, FRAMESIZE_QVGA);
    // s->set_whitebal(s, 1);
    // s->set_brightness(s, 1);
    // s->set_contrast(s, 2);
    // s->set_saturation(s, 1);
    // s->set_gain_ctrl(s, 1);
    // s->set_gainceiling(s, (gainceiling_t)6);
    // s->set_exposure_ctrl(s, 1);
    // s->set_denoise(s, 1);
    // s->set_lenc(s, 1);
    // */
    
    // Serial.printf("Sensor PID: 0x%04x\n", s->id.PID);
}

void loop() {
    digitalWrite(LED_BUILTIN, WiFi.status() == WL_CONNECTED);
    server.handleClient(); // Respond to HTTP requests

    // static unsigned long lastCapture = 0;
    // if (millis() - lastCapture > 500000) {  // Capture every 5 seconds
    //     lastCapture = millis();

    //     camera_fb_t * fb = esp_camera_fb_get();
    //     if (!fb) {
    //         Serial.println("JPEG capture failed: fb == NULL");
    //     } else {
    //         Serial.printf("Captured JPEG: %dx%d, %d bytes\n", fb->width, fb->height, fb->len);
    //         // Store the image data if needed
    //         esp_camera_fb_return(fb);
    //     }
    // }
}



// Wi-fi Stuff

// void setup() {
//     Serial.begin(115200);
//     pinMode(LED_BUILTIN, OUTPUT);

//     // addAP req. two args, (SSID, PASSWORD)
//     wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

//     // Keeps trying to connect to wifi until WL_CONNECTED signal given
//     while(wifiMulti.run() != WL_CONNECTED) {
//         delay(100);
//     }
    
//     // Serial Monitor Logging
//     Serial.println("Connected to Wifi");
//     Serial.println("Connected to Wifi");
//     Serial.print("IP address: ");
//     Serial.println(WiFi.localIP());
    
    
//     server.on("/", handleRoot); // Define route
//     server.begin();
    
//     Serial.print("HTTP server started");
// }

// void loop() {
//     digitalWrite(LED_BUILTIN, WiFi.status() == WL_CONNECTED);
//     server.handleClient(); // Handle incoming HTTP requests
// }