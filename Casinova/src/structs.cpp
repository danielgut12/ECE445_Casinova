#include "structs.h"

// Define global variables
std::map<String, Player> players;
GamePhase currentPhase = WAITING;
std::vector<Card> communityCards;
std::vector<String> playerOrder;  // Keeps order of joined players
int currentDealIndex = 0;         // Tracks who’s being dealt to now
int nextPlayerIndex = 1;
