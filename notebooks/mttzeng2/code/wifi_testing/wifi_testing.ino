#include <WiFi.h>

WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    delay(3000);

    WiFi.mode(WIFI_AP);
    
    // Start the AP with specific settings
    WiFi.softAP("ESP32-Network", "12345678", 6, 0, 4); // Channel 6, Visible, Max 4 Clients

    // Force 802.11b/g mode by setting lower bandwidth
    WiFi.setTxPower(WIFI_POWER_19_5dBm);  // Boost WiFi signal (optional)
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

    Serial.println("WiFi AP Started! IP: " + WiFi.softAPIP().toString());
}

void loop() {
    WiFiClient client = server.available();
    
    if (client) {
        Serial.println("New Client Connected.");
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println("Connection: close");
        client.println();
        client.println("<html><body><h1>ESP32 Web Server</h1></body></html>");
        client.println();
        client.stop();
    }
  Serial.println(WiFi.softAPIP());
    // delay(1000);
    // Serial.println("Looping...");
}
