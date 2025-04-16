#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "camera.h"

#define CONTROL_PIN 42  // GPIO 42
#define stepPin 1
#define dirPin  2
#define stepsPerRevolution 200

AsyncWebServer server(80);

void initWifi() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Casinova", "");

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        Serial.println("Device connected");
    }, ARDUINO_EVENT_WIFI_AP_STACONNECTED);

    Serial.println("Access Point started");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());

    // Optional home page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", R"rawliteral(
            <html>
            <body>
                <h2>Casinova Page!</h2>
                <button onclick="capture()">Capture Image</button>
                <br><br>
                <img id="snapshot" width="320"/>
                <script>
                    function capture() {
                        const img = document.getElementById('snapshot');
                        img.src = '/capture?t=' + new Date().getTime(); // prevent caching
                    }
                </script>
            </body>
            </html>
        )rawliteral");
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
                free(jpeg_buf);  // Free after last chunk
            }

            return to_copy;
        });

    response->addHeader("Cache-Control", "no-cache");
    request->send(response);
});

    server.begin();
}
















// void runMotorSequence() {
//     // Clockwise
//     digitalWrite(dirPin, HIGH);
//     for (int i = 0; i < 8; i++) {
//         for (int x = 0; x < stepsPerRevolution / 8; x++) {
//             digitalWrite(stepPin, HIGH);
//             delayMicroseconds(2000);
//             digitalWrite(stepPin, LOW);
//             delayMicroseconds(2000);
//         }
//         delay(1000);
//     }
  
//     // Counterclockwise
//     digitalWrite(dirPin, LOW);
//     for (int x = 0; x < stepsPerRevolution; x++) {
//         digitalWrite(stepPin, HIGH);
//         delayMicroseconds(2000);
//         digitalWrite(stepPin, LOW);
//         delayMicroseconds(2000);
//     }
  
//     delay(1000);
//   }

// void initWifi() {
//     WiFi.softAP("bumWifi", "");
//     Serial.println("Access Point started");
//     Serial.println(WiFi.softAPIP());

//     // pinMode(CONTROL_PIN, OUTPUT);
//     // digitalWrite(CONTROL_PIN, LOW);  // Start OFF
    


//     // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     //     request->send_P(200, "text/html", R"rawliteral(
//     // <!DOCTYPE html>
//     // <html>
//     // <head>
//     //     <title>Motor Control</title>
//     // </head>
//     // <body style="text-align:center; font-family:sans-serif; padding-top:50px;">
//     //     <h2>Stepper Motor Control</h2>
//     //     <button onclick="fetch('/motor')">Run Motor</button>
//     // </body>
//     // </html>
//     //     )rawliteral");
//     // });
    

//     // pinMode(stepPin, OUTPUT);
//     // pinMode(dirPin, OUTPUT);

//     // server.on("/motor", HTTP_GET, [](AsyncWebServerRequest *request){
//     //     runMotorSequence();
//     //     request->send(200, "text/plain", "Motor sequence complete");
//     // });


//     server.begin();
// }