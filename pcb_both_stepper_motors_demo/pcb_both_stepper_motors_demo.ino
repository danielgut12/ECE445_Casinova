int supplyStepPin = 39;
int supplyDirectionPin = 38;


int swivelStepPin = 5;
int swivelDirectionPin = 4;


int stepsPerRevolution = 200;
int delayT = 1000;

void setup() {
   // Declare pins as Outputs
  pinMode(swivelStepPin, OUTPUT);
  pinMode(swivelDirectionPin, OUTPUT);
  pinMode(supplyStepPin, OUTPUT);
  pinMode(supplyDirectionPin, OUTPUT);
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
  // Set motor direction clockwise
  digitalWrite(supplyDirectionPin, HIGH);

  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(supplyStepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(supplyStepPin, LOW);
    delayMicroseconds(2000);
  }

  // Wait a second
  delay(delayT);

//  for (int i = 0; i < 4; i++) {
//    makeQuarterTurn();
//    delay(1.5 * delayT);
//  }
//
//  digitalWrite(swivelDirectionPin, HIGH);
//  resetToStart();
//
//  // Wait a second
//  delay(delayT);
}
