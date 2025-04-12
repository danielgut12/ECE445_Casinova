#ifndef MOTORS_H
#define MOTORS_H

void initStepper();
void initMotors();
void rotateToPlayer(int index);
void pushCardToEjectionZone();
void ejectCardToDistance(float dist);

#endif