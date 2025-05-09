#include <Arduino.h>

int ejectionPinInput1 = 48;
int ejectionPinInput2 = 21;
int ejectionPWMPin = 47;


int supplyStepPin = 39;
int supplyDirectionPin = 38;
int stepsPerRevolution = 200;

int swivelStepPin = 5;
int swivelDirectionPin = 4;

void setup() {
  Serial.begin(115200);
  // --- Demo stuff ---
    // put your setup code here, to run once:
    pinMode(ejectionPinInput1, OUTPUT);
    pinMode(ejectionPinInput2, OUTPUT);
    pinMode(ejectionPWMPin,  OUTPUT);
  
    pinMode(supplyStepPin, OUTPUT);
    pinMode(supplyDirectionPin, OUTPUT);

    pinMode(swivelStepPin, OUTPUT);
    pinMode(swivelDirectionPin, OUTPUT);
  
  delay(100);
}

void ejectCard(int power) {
  analogWrite(ejectionPWMPin, power); //ENA  pin

  // turn on ejection motor
  digitalWrite(ejectionPinInput1,  LOW);
  digitalWrite(ejectionPinInput2, HIGH);
  delay(1000);

  // turn on supply motor
  digitalWrite(supplyDirectionPin, HIGH);

  // Spin motor quickly
  for(int x = 0; x < 4 * (stepsPerRevolution / 10); x++)
  {
    digitalWrite(supplyStepPin, HIGH);
    delayMicroseconds(4000);
    digitalWrite(supplyStepPin, LOW);
    delayMicroseconds(4000);
  }
  delay(500);

  // turn off ejection motor
  digitalWrite(ejectionPinInput1, LOW);
  digitalWrite(ejectionPinInput2, LOW);
  delay(1000);
}

void makeQuarterTurn() {
    digitalWrite(swivelDirectionPin, LOW);
  
    // Spin motor acceleration
    for(int x = 0; x < 10; x++)
    {
      digitalWrite(swivelStepPin, HIGH);
      delayMicroseconds(14000);
      digitalWrite(swivelStepPin, LOW);
      delayMicroseconds(14000);
    }
  
    // Spin motor
    for(int x = 0; x < 30; x++)
    {
      digitalWrite(swivelStepPin, HIGH);
      delayMicroseconds(12000);
      digitalWrite(swivelStepPin, LOW);
      delayMicroseconds(12000);
    }

     // Spin motor deacceleration
    for(int x = 0; x < 20; x++)
    {
      digitalWrite(swivelStepPin, HIGH);
      delayMicroseconds(14000);
      digitalWrite(swivelStepPin, LOW);
      delayMicroseconds(14000);
    }


    // Stopping the jiggle
    digitalWrite(swivelDirectionPin, HIGH);
    for(int x = 0; x < 2; x++)
    {
      digitalWrite(swivelStepPin, HIGH);
      delayMicroseconds(14000);
      digitalWrite(swivelStepPin, LOW);
      delayMicroseconds(14000);
    }
    digitalWrite(swivelDirectionPin, LOW);
}

void resetToStart() {
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(swivelStepPin, HIGH);
    delayMicroseconds(12000);
    digitalWrite(swivelStepPin, LOW);
    delayMicroseconds(12000);
  }
}

void loop() {

    for (int i = 0; i < 4; i++) {
        makeQuarterTurn();
        delay(1000);
        ejectCard(170);
    }
    
    digitalWrite(swivelDirectionPin, HIGH);
    resetToStart();

    delay(500);

}
