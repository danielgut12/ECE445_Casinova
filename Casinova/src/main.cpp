#include <WiFi.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>

#define AP_SSID "CardDetector"
#define AP_PASSWORD "casinova123"

AsyncWebServer server(80);
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Mount LittleFS
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed");
    return;
  }
  Serial.println("LittleFS mounted");

  // Start Access Point
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  // Serve the static HTML page
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  // Toggle LED handler
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request){
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
    request->send(200, "text/plain", ledState ? "ON" : "OFF");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
    // Blink LED every 500ms
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
  
