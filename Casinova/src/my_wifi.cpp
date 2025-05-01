#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "camera.h"
#include "motors.h"
#include "game.h"
#include "structs.h"
#include "tof_sensor.h"
#include <AsyncWebSocket.h>

AsyncWebSocket ws("/ws");  // WebSocket endpoint

AsyncWebServer server(80);

std::map<uint32_t, String> clientIdToPlayerId;  // WebSocket client ID → Player ID mapping
extern std::map<String, Player> players;        // already existing players map


// TOF Distance Sensor stuff
unsigned long lastDistanceSent = 0;
const unsigned long distanceInterval = 200; // every 200ms

void broadcastDistance() {
    unsigned long now = millis();
    if (now - lastDistanceSent >= distanceInterval) {
        lastDistanceSent = now;

        float distance = getPlayerDistance();
        if (distance >= 0.0) {
            StaticJsonDocument<64> doc;
            doc["type"] = "distance";
            doc["distance"] = distance;

            String json;
            serializeJson(doc, json);
            ws.textAll(json); // Send JSON to all connected clients
            // Serial.println("Distance sent: " + json);
        }
    }
}

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
    // WiFi.mode(WIFI_AP_STA);     // STA (router mode)
    WiFi.mode(WIFI_AP);


    // WiFi.begin("2.4 410GRN-U313", "CXNK001CBEAB");
    // Serial.print("Connecting to WiFi");
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(500);
    //     Serial.print(".");
    // }

    // Serial.println("\nConnected to router!");
    // Serial.print("Local IP: ");
    // Serial.println(WiFi.localIP());

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
        Serial.printf("WebSocket client connected: %u\n", client->id());
        } 

        else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket client disconnected: %u\n", client->id());

        uint32_t cid = client->id();
        if (clientIdToPlayerId.count(cid)) {
            String id = clientIdToPlayerId[cid];
            players.erase(id);
            clientIdToPlayerId.erase(cid);

            broadcastPlayerList();
            Serial.printf("Player %s removed after disconnect\n", id.c_str());
        }
        }

        else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo*)arg;

        if (info->final && info->index == 0 && info->len == len) {
            String msg = "";
            for (size_t i = 0; i < len; i++) {
                msg += (char)data[i];
            }

            Serial.printf("[DEBUG] Received WebSocket message: %s\n", msg.c_str());  // ADD THIS
            StaticJsonDocument<200> doc;
            DeserializationError err = deserializeJson(doc, msg);
            if (!err) {
                if (doc["type"] == "identify") {
                    String id = doc["id"];
                    clientIdToPlayerId[client->id()] = id;
                    Serial.printf("Client %u linked to player %s\n", client->id(), id.c_str());
                }
            }
        }
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
    server.on("/demoEject", HTTP_GET, [](AsyncWebServerRequest *request){
        ejectCard(150);
        request->send(200, "text/plain", "Eject Demo");
    });

    server.on("/rotateQuarterCW", HTTP_GET, [](AsyncWebServerRequest *request){
        makeQuarterTurnCW();
        request->send(200, "text/plain", "Eject Demo");
    });
    
    server.on("/rotateQuarterCCW", HTTP_GET, [](AsyncWebServerRequest *request){
        makeQuarterTurnCCW();
        request->send(200, "text/plain", "Eject Demo");
    });

    server.on("/rotateReset", HTTP_GET, [](AsyncWebServerRequest *request){
        resetToStart();
        request->send(200, "text/plain", "Eject Demo");
    });

    // Ejection with TOF sensor
    server.on("/autoEject", HTTP_GET, [](AsyncWebServerRequest *request){
        pushCardsWithDistance();
        request->send(200, "text/plain", "Auto Ejection triggered");
    });


    server.on("/queueCard", HTTP_GET, [](AsyncWebServerRequest *request){
        queueCard();
        request->send(200, "text/plain", "Auto Ejection triggered");
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
        String id;
    
        if (request->hasParam("name")) {
            id = request->getParam("name")->value();
        } else {
            id = "player" + String(nextPlayerIndex++);
        }
    
        if (!players.count(id)) {
            Player p;
            p.id = id;
            players[id] = p;
            playerOrder.push_back(id);
        }
    
        broadcastPlayerList();
        request->send(200, "text/plain", id);
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


        // ready button toggle logic
        players[id].ready = !players[id].ready;

        bool allReady = players.size() >= 2; // SHOULD BE 2 BUT I MEAN TESTING
        for (auto& p : players) {
            if (!p.second.ready) {
                allReady = false;
                break;
            }
        }

        if (allReady) {
            Serial.println("All players ready. starting game");
            nextPhase();
            broadcastGameState();
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

    server.on("/remove", HTTP_GET, [](AsyncWebServerRequest *request){
        if (!request->hasParam("id")) {
            request->send(400, "text/plain", "Missing id");
            return;
        }
    
        String id = request->getParam("id")->value();
        if (!players.count(id)) {
            request->send(404, "text/plain", "Player not found");
            return;
        }
    
        // Find and erase from players
        players.erase(id);
    
        // Also remove from clientIdToPlayerId map if exists
        for (auto it = clientIdToPlayerId.begin(); it != clientIdToPlayerId.end(); ++it) {
            if (it->second == id) {
                clientIdToPlayerId.erase(it);
                break;
            }
        }
    
        broadcastPlayerList();
        Serial.printf("Player %s manually removed\n", id.c_str());
        request->send(200, "text/plain", "Player " + id + " removed");
    });
    


    server.on("/hand", HTTP_GET, [](AsyncWebServerRequest *request){
        if (!request->hasParam("id")) {
            request->send(400, "text/plain", "Missing id");
            return;
        }
        String id = request->getParam("id")->value();
        Serial.printf("[DEBUG] Hand requested for player: %s\n", id.c_str());
        if (!players.count(id)) {
            Serial.println("[DEBUG] Player not found!");
            request->send(404, "text/plain", "Player not found");
            return;
        }
    
        Player& p = players[id];
    
        StaticJsonDocument<256> doc;
        if (p.hasCard1) {
            doc["card1"] = cardToString(p.card1);
        }
        if (p.hasCard2) {
            doc["card2"] = cardToString(p.card2);
        }
    
        String json;
        serializeJson(doc, json);
        request->send(200, "application/json", json);
    });

    server.on("/fold", HTTP_POST, [](AsyncWebServerRequest *request){
        if (!request->hasParam("id", true)) {
            request->send(400, "text/plain", "Missing player ID");
            return;
        }
    
        String playerId = request->getParam("id", true)->value();
        if (players.count(playerId)) {
            players[playerId].active = false;
            players[playerId].action = "fold";
            Serial.println(playerId + " has folded.");

            onPlayerAction(playerId, "fold"); // triggers player action with fold action
            request->send(200, "text/plain", "Folded");
            broadcastGameState();
        } else {
            request->send(404, "text/plain", "Player not found");
        }
    });
    
    

    server.begin();
}
