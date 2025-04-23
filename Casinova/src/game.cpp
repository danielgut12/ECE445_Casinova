#include "game.h"
#include "structs.h"
#include "motors.h"
#include "camera.h"
#include "deck.h"


// Phases for poker game
void nextPhase() {
    switch (currentPhase) {
        case WAITING:
            if (players.size() >= 2) currentPhase = PREFLOP;
            break;
        case PREFLOP:
            dealHoleCards();
            currentPhase = FLOP;
            break;
        case FLOP:
            dealCommunityCards(3);

            currentPhase = TURN;
            break;
        case TURN:
            dealCommunityCards(1);
            currentPhase = RIVER;
            break;
        case RIVER:
            dealCommunityCards(1);
            currentPhase = SHOWDOWN;
            break;
        case SHOWDOWN:
            // displayFinalHands(); // Optional
            currentPhase = RESET;
            break;
        case RESET:
            resetGame();
            currentPhase = WAITING;
            break;
    }

    clearPlayerActions();
    // notifyClients();  // Optional: use SSE/WebSocket/polling
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
    playerOrder.clear();
    currentDealIndex = 0;
    players.clear();
}

/*
void dealHoleCards() {
    for (int i = 0; i < playerOrder.size(); ++i) {
        const String& id = playerOrder[i];
        Player& p = players[id];
        if (!p.active) continue;
        
        // Uncomment once motor is working
        // rotateToPlayer(i, playerOrder.size());
        
        Card card;
        
        // pushCardsWithDistance();     // First card
        // captureAndDetectCard(card);  // Fills via reference
        card = drawCard();
        p.card1 = card;
        p.hasCard1 = true;
        
        // pushCardsWithDistance();     // Second card
        // captureAndDetectCard(card);
        card = drawCard();
        p.card2 = card;
        p.hasCard2 = true;
    }
    
    // Serial print cards
    for (auto& id : playerOrder) {
        Player& p = players[id];
        Serial.printf("Player %s: %d of %d and %d of %d\n",
        id.c_str(),
        p.card1.rank, p.card1.suit,
        p.card2.rank, p.card2.suit);
    }
    
}

*/

void dealHoleCards() {
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
