#include "my_wifi.h"
#include "camera.h"
AsyncWebServerResponse *mjpegResponse = nullptr;

AsyncWebServer server(80);


void handleMjpegStream(AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginChunkedResponse(
        "multipart/x-mixed-replace; boundary=frame",
        [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
            uint8_t* jpg_buf = nullptr;
            size_t jpg_len = 0;

            if (!getJpegFrame(&jpg_buf, &jpg_len) || jpg_len == 0)
                return 0;

            size_t headerLen = snprintf((char *)buffer, maxLen,
                "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", jpg_len);

            if (headerLen + jpg_len + 2 > maxLen) {
                free(jpg_buf);
                return 0;
            }

            memcpy(buffer + headerLen, jpg_buf, jpg_len);
            size_t totalLen = headerLen + jpg_len;
            memcpy(buffer + totalLen, "\r\n", 2);
            totalLen += 2;

            free(jpg_buf);
            return totalLen;
        });

        // anti caching method
        response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
        response->addHeader("Pragma", "no-cache");
        response->addHeader("Expires", "-1");

    request->send(response);
}

    
void initWifi() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("bumWifi", "");

    Serial.println("Access Point started");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Optional home page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", R"rawliteral(
            <html><body>
            <h2>ESP32 MJPEG Stream</h2>
            <img src="/stream" width="320" />
            </body></html>
        )rawliteral");
    });

    // Optional capture test route
    server.on("/capture", HTTP_GET, [](AsyncWebServerRequest *request){
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            request->send(500, "text/plain", "Camera capture failed");
            return;
        }

        AsyncWebServerResponse *response = request->beginResponse("image/jpeg", fb->len, 
            [fb](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
                size_t to_copy = (index + maxLen > fb->len) ? (fb->len - index) : maxLen;
                memcpy(buffer, fb->buf + index, to_copy);
                if (index + to_copy == fb->len) esp_camera_fb_return(fb);
                return to_copy;
            });

        response->addHeader("Cache-Control", "no-cache");
        request->send(response);
    });

    server.begin();
}
