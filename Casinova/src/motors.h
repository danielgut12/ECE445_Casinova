#ifndef MOTORS_H
#define MOTORS_H

void setupSwivel();
void rotateToPlayer(int playerIndex, int totalPlayers);
void rotateByDegrees(float degrees);
void setupEjection();
void runEjection(int distance_mm);

#endif
