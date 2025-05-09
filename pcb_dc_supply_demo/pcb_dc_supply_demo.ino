int ejectionPinInput1 = 48;
int ejectionPinInput2 = 21;
int ejectionPWMPin = 47;


int supplyStepPin = 39;
int supplyDirectionPin = 38;
int stepsPerRevolution = 200;

void setup() {
  // put your setup code here, to run once:
  pinMode(ejectionPinInput1, OUTPUT);
  pinMode(ejectionPinInput2, OUTPUT);
  pinMode(ejectionPWMPin,  OUTPUT);

  pinMode(supplyStepPin, OUTPUT);
  pinMode(supplyDirectionPin, OUTPUT);

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


void loop() {
  ejectCard(100);
  delay(500);
}
