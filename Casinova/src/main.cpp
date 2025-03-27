#include <Arduino.h>
#include "esp_camera.h"
#include "base64.h"

#define TAG "CAM"

// WROVER-KIT PIN Map
#define CAM_PIN_PWDN    -1
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK    13
#define CAM_PIN_SIOD    9
#define CAM_PIN_SIOC    10

#define CAM_PIN_D7      18
#define CAM_PIN_D6      17
#define CAM_PIN_D5      16
#define CAM_PIN_D4      15
#define CAM_PIN_D3       7
#define CAM_PIN_D2       6
#define CAM_PIN_D1       5
#define CAM_PIN_D0       4
#define CAM_PIN_VSYNC   11
#define CAM_PIN_HREF    12
#define CAM_PIN_PCLK    14

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

    .xclk_freq_hz = 16000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_RGB565,
    .frame_size = FRAMESIZE_QVGA,
    .jpeg_quality = 10,
    .fb_count = 1,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY
};

void process_image(int width, int height, pixformat_t format, uint8_t *buf, size_t len) {
    Serial.printf("Image captured: %dx%d, format=%d, size=%d bytes\n", width, height, format, len);
    String encoded = base64::encode(buf, len);
    Serial.println(encoded); // Base64 encoded RGB565
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
        Serial.println("Camera Capture Failed");
        return ESP_FAIL;
    } else {
        // Serial.printf("Captured %dx%d, %d bytes\n", fb->width, fb->height, fb->len);
    }

    process_image(fb->width, fb->height, fb->format, fb->buf, fb->len);

    esp_camera_fb_return(fb);
    return ESP_OK;
}



void setup() {
    Serial.begin(115200);
    delay(1000); // Allow time for serial monitor

    if (!psramFound()) {
        Serial.println("PSRAM not found!");
    }

    if (camera_init() == ESP_OK) {
        Serial.println("Camera initialized successfully");
    } else {
        Serial.println("Camera initialization failed");
    }
    Serial.printf("PSRAM: %s, size: %d bytes\n", psramFound() ? "OK" : "NOT FOUND", ESP.getPsramSize());
    sensor_t * s = esp_camera_sensor_get();

    s->set_whitebal(s, 1);
    s->set_brightness(s, 1);
    s->set_contrast(s, 2);
    s->set_saturation(s, 1);
    s->set_gain_ctrl(s, 1);
    s->set_gainceiling(s, (gainceiling_t)6);
    s->set_exposure_ctrl(s, 1);
    s->set_denoise(s, 1);
    s->set_lenc(s, 1);
    
    Serial.printf("Sensor PID: 0x%04x\n", s->id.PID);
}

void loop() {
    camera_capture();
    delay(10000000); // Capture every 10 seconds
}
