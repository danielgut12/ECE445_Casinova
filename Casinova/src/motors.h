#ifndef MOTORS_H
#define MOTORS_H

void setupSwivel();
void rotateToPlayer(int playerIndex, int totalPlayers);
void setupEjection();
void runEjection(int distance_mm);

#endif
