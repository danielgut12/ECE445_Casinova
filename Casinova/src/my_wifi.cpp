#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "camera.h"
#include "motors.h"

AsyncWebServer server(80);

void initWifi() {
    WiFi.mode(WIFI_AP_STA);

    // STA (router mode)
    WiFi.begin("2.4 410GRN-U313", "CXNK001CBEAB");
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected to router!");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());

    // AP (access point)
    WiFi.softAP("Casinova", "");
    Serial.println("Access Point started");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        Serial.println("Device connected to AP");
    }, ARDUINO_EVENT_WIFI_AP_STACONNECTED);

    // Initialize LittleFS
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }

    // Routes
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", String(), false);
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/style.css", "text/css");
    });

    server.on("/capture", HTTP_GET, [](AsyncWebServerRequest *request){
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            request->send(500, "text/plain", "Camera capture failed");
            return;
        }

        uint8_t* jpeg_buf = nullptr;
        size_t jpeg_len = 0;

        if (!frame2jpg(fb, 80, &jpeg_buf, &jpeg_len)) {
            esp_camera_fb_return(fb);
            request->send(500, "text/plain", "JPEG conversion failed");
            return;
        }

        esp_camera_fb_return(fb);

        AsyncWebServerResponse *response = request->beginResponse("image/jpeg", jpeg_len,
            [jpeg_buf, jpeg_len](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
                size_t to_copy = (index + maxLen > jpeg_len) ? (jpeg_len - index) : maxLen;
                memcpy(buffer, jpeg_buf + index, to_copy);
                if (index + to_copy == jpeg_len) {
                    free(jpeg_buf);
                }
                return to_copy;
            });

        response->addHeader("Cache-Control", "no-cache");
        request->send(response);
    });

    server.on("/eject", HTTP_GET, [](AsyncWebServerRequest *request){
        if (!request->hasParam("distance")) {
            request->send(400, "text/plain", "Missing distance");
            return;
        }

        int distance = request->getParam("distance")->value().toInt();
        runEjection(distance);
        request->send(200, "text/plain", "Ejection triggered at " + String(distance) + " mm");
    });

    server.begin();
}
