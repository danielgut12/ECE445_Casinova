#include <ArduinoJson.h>     // for StaticJsonDocument, serializeJson
#include <AsyncWebSocket.h>  // for ws.textAll()
#include "game.h"
#include "structs.h"
#include "motors.h"
#include "camera.h"
#include "deck.h"

extern AsyncWebSocket ws;

void broadcastGameState() {
    StaticJsonDocument<512> doc;

    // Current phase (e.g., "Pre-Flop", "Flop", etc.)
    switch (currentPhase) {
        case WAITING: doc["phase"] = "Waiting"; break;
        case PREFLOP: doc["phase"] = "Pre-Flop"; break;
        case FLOP: doc["phase"] = "Flop"; break;
        case TURN: doc["phase"] = "Turn"; break;
        case RIVER: doc["phase"] = "River"; break;
        case SHOWDOWN: doc["phase"] = "Showdown"; break;
        case RESET: doc["phase"] = "Reset"; break;
    }

    // Community cards
    JsonArray community = doc.createNestedArray("community");
    for (const auto& card : communityCards) {
        community.add(cardToString(card)); // e.g., "AS" = Ace of Spades
    }

    // Serialize and send over WebSocket
    String json;
    serializeJson(doc, json);
    ws.textAll(json);
}


// Phases for poker game
void nextPhase() {
    if (currentPhase == WAITING) {
        Serial.println("Waiting");
        if (players.size() >= 2) {
            currentPhase = PREFLOP;
        } else {
            // Not enough players, stay in WAITING
            broadcastGameState();
            return;
        }
    }

    switch (currentPhase) {
        case PREFLOP:
            Serial.println("PreFlop");
            dealHoleCards();
            currentPhase = FLOP;
            break;

        case FLOP:
            Serial.println("Flop");
            dealCommunityCards(3);
            currentPhase = TURN;
            break;

        case TURN:
            Serial.println("Turn");
            dealCommunityCards(1);
            currentPhase = RIVER;
            break;

        case RIVER:
            Serial.println("River");
            dealCommunityCards(1);
            currentPhase = SHOWDOWN;
            break;

        case SHOWDOWN:
            Serial.println("Showdown");
            // Optionally display final hands
            currentPhase = RESET;
            break;

        case RESET:
            Serial.println("Resetting game");
            resetGame();
            currentPhase = WAITING;
            break;

        default:
            Serial.println("Unknown phase");
            currentPhase = WAITING;
            break;
    }

    clearPlayerActions();
    broadcastGameState();
}


void onPlayerAction(const String& id, const String& action) {
    if (players.count(id)) {
        players[id].action = action;
        if (action == "fold") players[id].active = false;
    }

    if (allActivePlayersActed()) {
        nextPhase();
    }
}

bool allActivePlayersActed() {
    for (auto &[id, p] : players)
        if (p.active && p.action == "") return false;
    return true;
}

void clearPlayerActions() {
    for (auto &[id, p] : players) p.action = "";
}

void resetGame() {
    currentDealIndex = 0;

    // Rotate playerOrder so dealer moves
    if (!playerOrder.empty()) {
        String firstPlayer = playerOrder.front();
        playerOrder.erase(playerOrder.begin());
        playerOrder.push_back(firstPlayer);
    }

    for (auto& pair : players) {
        Player& p = pair.second;
        p.card1 = Card{};      // Clear card1
        p.card2 = Card{};      // Clear card2
        p.hasCard1 = false;
        p.hasCard2 = false;
        p.action = "";
        p.active = true;
        p.ready = true;       // Optional: force them to click "Ready" again
    }

    communityCards.clear();   // Clear board cards
    initMockDeck();           // Reshuffle deck
    Serial.println("Players after reset:");
    for (const auto& pair : players) {
        Serial.println(pair.first);  // Print player ID
    }
}

void dealHoleCards() {
    Serial.printf("[DEBUG] Dealing cards to %d players\n", playerOrder.size());
    for (int i = 0; i < playerOrder.size(); ++i) {
        const String& id = playerOrder[i];
        Player& p = players[id];
        if (!p.active) continue;

        Card card1 = drawCard();
        Card card2 = drawCard();

        p.card1 = card1;
        p.card2 = card2;
        p.hasCard1 = true;
        p.hasCard2 = true;

        Serial.printf("Player %s got: %s, %s\n",
            id.c_str(),
            cardToString(p.card1).c_str(),
            cardToString(p.card2).c_str());
    }

    //  AFTER dealing, notify all players
    StaticJsonDocument<64> doc;
    doc["event"] = "newHand";

    String json;
    serializeJson(doc, json);
    ws.textAll(json);  //  Push event to all WebSocket clients
}


void dealCommunityCards(int count) {
    for (int i = 0; i < count; ++i) {
        // rotateToBoard();
        // pushCardsWithDistance();
        
        Card card = drawCard();
        // captureAndDetectCard(card);
        communityCards.push_back(card);
    }
    
}
