#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "camera.h"
#include "motors.h"
#include "game.h"
#include "structs.h"
#include <AsyncWebSocket.h>

AsyncWebSocket ws("/ws");  // WebSocket endpoint

AsyncWebServer server(80);


void broadcastPlayerList() {
    StaticJsonDocument<512> doc;
    JsonArray arr = doc.createNestedArray("players");

    for (auto& p : players) {
        JsonObject obj = arr.createNestedObject();
        obj["id"] = p.second.id;
        obj["ready"] = p.second.ready;
    }

    String json;
    serializeJson(doc, json);
    ws.textAll(json);  // Broadcast to all WebSocket clients
}


void initWifi() {
    WiFi.mode(WIFI_AP_STA);     // STA (router mode)
    // WiFi.mode(WIFI_AP);


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


    // Web Sockets
    ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, 
    AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
    Serial.printf("WebSocket client %u connected\n", client->id());
    } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("WebSocket client %u disconnected\n", client->id());
    }
    });
    server.addHandler(&ws);

    // Initialize LittleFS
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }

    // Routes
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", String(), false);
    });

    server.on("/debug", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/debug.html", String(), false);
    });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/script.js", "application/javascript");
    });
    

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/style.css", "text/css");
    });

    server.on("/capture", HTTP_GET, [](AsyncWebServerRequest *request){
        uint8_t* jpeg_buf = nullptr;
        size_t jpeg_len = 0;

        if (!getJpegFrame(&jpeg_buf, &jpeg_len)) {
            request->send(500, "text/plain", "Frame grab failed");
            return;
        }

        AsyncWebServerResponse *response = request->beginResponse("image/jpeg", jpeg_len,
            [jpeg_buf, jpeg_len](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
                size_t to_copy = (index + maxLen > jpeg_len) ? (jpeg_len - index) : maxLen;
                memcpy(buffer, jpeg_buf + index, to_copy);
                if (index + to_copy == jpeg_len) free(jpeg_buf);
                return to_copy;
            });

        response->addHeader("Cache-Control", "no-cache");
        request->send(response);
    });



    // Ejection route
    server.on("/eject", HTTP_GET, [](AsyncWebServerRequest *request){
        if (!request->hasParam("distance")) {
            request->send(400, "text/plain", "Missing distance");
            return;
        }

        int distance = request->getParam("distance")->value().toInt();
        runEjection(distance);
        request->send(200, "text/plain", "Ejection triggered at " + String(distance) + " mm");
    });

    // Rotation route
    server.on("/rotate", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("degrees")) {
            float deg = request->getParam("degrees")->value().toFloat();
            rotateByDegrees(deg);  // ← Make sure this function is defined somewhere
            request->send(200, "text/plain", "Rotated " + String(deg) + " degrees.");
        } else {
            request->send(400, "text/plain", "Missing 'degrees' parameter.");
        }
    });

    server.on("/join", HTTP_GET, [](AsyncWebServerRequest *request){
        String name = request->hasParam("name") ? request->getParam("name")->value() : "player" + String(nextPlayerIndex++);
        if (players.count(name) == 0) {
            Player p;
            p.id = name;
            players[name] = p;
            playerOrder.push_back(name);
            Serial.println("New player joined: " + name);
        }

        // broadcast BEFORE you send req
        broadcastPlayerList();
        request->send(200, "text/plain", name);
    });
    
    
    // Action for Poker Route
    server.on("/action", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("POST /action triggered");
        if (request->hasParam("plain", true)) {
            String body = request->getParam("plain", true)->value();
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, body);
    
            if (!error) {
                String id = doc["id"];
                String action = doc["action"];
                onPlayerAction(id, action);
                request->send(200, "text/plain", "Action received");
            } else {
                request->send(400, "text/plain", "Invalid JSON");
            }
        } else {
            request->send(400, "text/plain", "No body");
        }
    });

    // TESTING: increment phase
    server.on("/nextPhase", HTTP_GET, [](AsyncWebServerRequest *request){
        nextPhase();
        request->send(200, "text/plain", "Phase advanced to: " + String(currentPhase));
    });

    server.on("/gamestate", HTTP_GET, [](AsyncWebServerRequest *request){
        StaticJsonDocument<512> doc;
        doc["phase"] = currentPhase;
    
        JsonArray cardsArray = doc.createNestedArray("community");
        for (const Card& c : communityCards) {
            cardsArray.add(cardToString(c));
        }
        
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });
    

    server.on("/ready", HTTP_POST, [](AsyncWebServerRequest *request) { }, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<128> doc;
        if (deserializeJson(doc, data, len)) {
            request->send(400, "text/plain", "Invalid JSON");
            return;
        }

        String id = doc["id"];
        if (!players.count(id)) {
            request->send(404, "text/plain", "Player not found");
            return;
        }

        players[id].ready = true;

        bool allReady = players.size() >= 2;
        for (auto& p : players) {
            if (!p.second.ready) {
                allReady = false;
                break;
            }
        }

        if (allReady) {
            // advanceGame();
        }

        // broadcast BEFORE you req
        broadcastPlayerList();
        request->send(200, "text/plain", "Marked ready");
    });

    server.on("/players", HTTP_GET, [](AsyncWebServerRequest *request){
        StaticJsonDocument<512> doc;
        JsonArray arr = doc.createNestedArray("players");

        for (const auto& p : players) {
            JsonObject obj = arr.createNestedObject();
            obj["id"] = p.second.id;
            obj["ready"] = p.second.ready;
        }

        String json;
        serializeJson(doc, json);

        // broadcast BEFORE send req
        broadcastPlayerList();
        request->send(200, "application/json", json);
    });

    

    server.begin();
}
