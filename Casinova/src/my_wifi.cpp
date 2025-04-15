#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define CONTROL_PIN 42  // GPIO 42
#define stepPin 1
#define dirPin  2
#define stepsPerRevolution 200

AsyncWebServer server(80);

void runMotorSequence() {
    // Clockwise
    digitalWrite(dirPin, HIGH);
    for (int i = 0; i < 8; i++) {
        for (int x = 0; x < stepsPerRevolution / 8; x++) {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(2000);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(2000);
        }
        delay(1000);
    }
  
    // Counterclockwise
    digitalWrite(dirPin, LOW);
    for (int x = 0; x < stepsPerRevolution; x++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(2000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(2000);
    }
  
    delay(1000);
  }

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
    <head>
        <title>Motor Control</title>
    </head>
    <body style="text-align:center; font-family:sans-serif; padding-top:50px;">
        <h2>Stepper Motor Control</h2>
        <button onclick="fetch('/motor')">Run Motor</button>
    </body>
    </html>
        )rawliteral");
    });
    

    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);

    server.on("/motor", HTTP_GET, [](AsyncWebServerRequest *request){
        runMotorSequence();
        request->send(200, "text/plain", "Motor sequence complete");
    });


    server.begin();
}


