#ifndef MY_WIFI_H
#define MY_WIFI_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

void handleMjpegStream(AsyncWebServerRequest *request);  // MJPEG stream handler
void capture_and_send();
void initWifi();
extern AsyncWebServer server;

#endif
