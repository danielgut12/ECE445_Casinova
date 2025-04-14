#include <Arduino.h>
#include "camera.h"
#include "base64.h"
#include "esp_log.h"

#define CAM_PIN_PWDN    -1
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK    12
#define CAM_PIN_SIOD    13
#define CAM_PIN_SIOC    14
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
#define CAM_PIN_PCLK    11

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
    .pixel_format = PIXFORMAT_GRAYSCALE,
    .frame_size = FRAMESIZE_QVGA,
    .jpeg_quality = 12,
    .fb_count = 2,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY
};

esp_err_t init_camera() {
    if (CAM_PIN_PWDN != -1) {
        pinMode(CAM_PIN_PWDN, OUTPUT);
        digitalWrite(CAM_PIN_PWDN, LOW);
    }

    delay(100);
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        Serial.println("Camera Init Failed");
        return err;
    }

    Serial.println("Camera initialized successfully");
    Serial.printf("PSRAM: %s, size: %d bytes\n", psramFound() ? "OK" : "NOT FOUND", ESP.getPsramSize());

    sensor_t * s = esp_camera_sensor_get();
    if (!s) {
        Serial.println("Failed to get sensor object");
        return ESP_FAIL;
    }

    s->set_pixformat(s, PIXFORMAT_GRAYSCALE);
    s->set_brightness(s, -2);
    s->set_contrast(s, 2);
    s->set_saturation(s, 0);
    s->set_exposure_ctrl(s, 0);
    s->set_aec_value(s, 150);
    s->set_gain_ctrl(s, 1);
    s->set_agc_gain(s, 5);
    s->set_whitebal(s, 0);
    s->set_sharpness(s, 2);

    Serial.printf("Sensor PID: 0x%04x\n", s->id.PID);
    return ESP_OK;
}

esp_err_t capture_and_send() {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera Capture Failed: framebuffer is NULL");
        return ESP_FAIL;
    }

    Serial.printf("Captured Image: %dx%d, format=%d, size=%d bytes\n", fb->width, fb->height, fb->format, fb->len);
    String encoded = base64::encode(fb->buf, fb->len);
    Serial.println(encoded);
    Serial.println("Done Printing Encoded");
    esp_camera_fb_return(fb);
    return ESP_OK;
}
