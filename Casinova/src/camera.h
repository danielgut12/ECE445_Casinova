#ifndef CAMERA_H
#define CAMERA_H

#include "esp_err.h"
#include "esp_camera.h"

// Initializes camera and sets sensor options
esp_err_t init_camera();

// Captures a grayscale image, base64 encodes it, and prints to Serial
esp_err_t capture_and_send();

#endif
