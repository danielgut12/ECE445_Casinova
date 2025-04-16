#ifndef CAMERA_H
#define CAMERA_H

#include "esp_err.h"
#include "esp_camera.h"

// Initializes camera and sets sensor options
esp_err_t init_camera();

bool getJpegFrame(uint8_t** jpeg_buf, size_t* jpeg_len);
esp_err_t capture_and_send();

#endif
