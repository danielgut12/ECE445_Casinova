#ifndef GAME_H
#define GAME_H

#include <Arduino.h>

void nextPhase();
void onPlayerAction(const String& id, const String& action);
void resetGame();
void dealHoleCards();
void dealCommunityCards(int count);
void clearPlayerActions();
bool allActivePlayersActed();

#endif
