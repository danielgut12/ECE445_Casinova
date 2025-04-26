#ifndef STRUCTS_H
#define STRUCTS_H

#include <Arduino.h>
#include <map>
#include <vector>
#include "card.h"

enum GamePhase {
    WAITING,
    PREFLOP,
    FLOP,
    TURN,
    RIVER,
    SHOWDOWN,
    RESET
};

struct Player {
    String id;
    String action = "";
    Card card1;
    Card card2;
    bool hasCard1 = false;
    bool hasCard2 = false;
    bool active = true; // if folded, false
    bool ready = false;
};

// Declare global variables
extern std::map<String, Player> players;
extern GamePhase currentPhase;
extern std::vector<Card> communityCards;
extern std::vector<String> playerOrder;
extern int currentDealIndex;

extern int nextPlayerIndex;
#endif
