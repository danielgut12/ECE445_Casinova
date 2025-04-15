#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define CONTROL_PIN 42  // GPIO 42

AsyncWebServer server(80);

void initWifi() {
    WiFi.softAP("bumWifi", "");
    Serial.println("Access Point started");
    Serial.println(WiFi.softAPIP());

    pinMode(CONTROL_PIN, OUTPUT);
    digitalWrite(CONTROL_PIN, LOW);  // Start OFF

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html>
<head><title>GPIO Toggle</title></head>
<body style="text-align:center; font-family:sans-serif; padding-top:50px;">
    <h2>GPIO 42 Control</h2>
    <button onclick="fetch('/on')">Turn ON</button>
    <button onclick="fetch('/off')">Turn OFF</button>
</body>
</html>
        )rawliteral");
    });

    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
        digitalWrite(CONTROL_PIN, HIGH);
        request->send(200, "text/plain", "ON");
    });

    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
        digitalWrite(CONTROL_PIN, LOW);
        request->send(200, "text/plain", "OFF");
    });

    server.begin();
}
