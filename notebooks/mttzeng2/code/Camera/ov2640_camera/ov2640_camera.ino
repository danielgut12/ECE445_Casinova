
/*#include <Wire.h>

#define SDA_PIN 8
#define SCL_PIN 3

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    Serial.println("Reading SCCB Registers...");

    Wire.beginTransmission(0x30);
    if (Wire.endTransmission() == 0) {
        Serial.println("OV2640 SCCB Communication OK!");
    } else {
        Serial.println("SCCB Error - Camera Not Responding!");
    }
}

void loop() {}
*/
// /*
#include "esp_camera.h"

#define XCLK_PIN 21  // GPIO for XCLK (MCLK)

// GPIO Pin Mapping (Use your setup)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM    12
#define XCLK_GPIO_NUM     21
#define SIOD_GPIO_NUM     8
#define SIOC_GPIO_NUM     3

#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       17
#define Y5_GPIO_NUM       16
#define Y4_GPIO_NUM       15
#define Y3_GPIO_NUM        7
#define Y2_GPIO_NUM        6
#define Y1_GPIO_NUM        5
#define Y0_GPIO_NUM        4
#define VSYNC_GPIO_NUM    10
#define HREF_GPIO_NUM      9
#define PCLK_GPIO_NUM     11

void resetCamera() {
    if (RESET_GPIO_NUM != -1) {
        pinMode(RESET_GPIO_NUM, OUTPUT);
        digitalWrite(RESET_GPIO_NUM, LOW);
        delay(10);
        digitalWrite(RESET_GPIO_NUM, HIGH);
        delay(10);
    }
}

// Web Server
#include "WiFi.h"
#include "WebServer.h"

const char* ssid = "MyPersonalHotspot";
const char* password = "hihihihi";
WebServer server(80);

void setup() {
    Serial.begin(115200);
  // // Generate 8MHz clock
  //   Serial.println("Generating 8 MHz clock...");

  //   // Attach XCLK using the new LEDC API
  //   if (ledcAttach(XCLK_PIN, 8000000, 1)) {
  //       Serial.println("XCLK running at 8 MHz!");
  //   } else {
  //       Serial.println("XCLK setup failed!");
  //   }

  // Web Server
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting...");
    }

    Serial.print("Connected! View image at: http://");
    Serial.println(WiFi.localIP());

    server.on("/capture", HTTP_GET, handleCapture);
    server.begin();

  // OV2640
    Serial.println("Initializing OV2640...");
  // Config Camera
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y0_GPIO_NUM;
    config.pin_d1 = Y1_GPIO_NUM;
    config.pin_d2 = Y2_GPIO_NUM;
    config.pin_d3 = Y3_GPIO_NUM;
    config.pin_d4 = Y4_GPIO_NUM;
    config.pin_d5 = Y5_GPIO_NUM;
    config.pin_d6 = Y6_GPIO_NUM;
    config.pin_d7 = Y7_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 24000000;  // Set XCLK to 24MHz
    config.pixel_format = PIXFORMAT_GRAYSCALE;  // Grayscale lowest size
    config.frame_size = FRAMESIZE_QQVGA;  // Use lowest resolution (160x120)
    config.fb_location = CAMERA_FB_IN_DRAM; // Store the frame buffer in the internal RAM (since i dont have PSRAM)
    config.fb_count = 1;
    config.jpeg_quality = 12;  // Adjust for quality (lower = better quality)

    resetCamera();
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed! Error 0x%x\n", err);
        return;
    }
    Serial.println("Camera Ready!");
}



void handleCapture() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        server.send(500, "text/plain", "Camera capture failed");
        return;
    }

    server.send_P(200, "image/jpeg", (const char*)fb->buf, fb->len);
    esp_camera_fb_return(fb);
}

void loop() {
    handleCapture();
    delay(10000);
}
// */
