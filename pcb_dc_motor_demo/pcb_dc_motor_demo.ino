int ejectionPinInput1 = 48;
int ejectionPinInput2 = 21;
int ejectionPWMPin = 47;

void setup() {
  // put your setup code here, to run once:
  pinMode(ejectionPinInput1, OUTPUT);
  pinMode(ejectionPinInput2, OUTPUT);
  pinMode(ejectionPWMPin,  OUTPUT);

}

void loop() {
  digitalWrite(ejectionPWMPin, HIGH);
  
  digitalWrite(ejectionPinInput1,  LOW);
  digitalWrite(ejectionPinInput2, HIGH);
  delay(3000);

  digitalWrite(ejectionPinInput1, LOW);
  digitalWrite(ejectionPinInput2, LOW);
  delay(3000);
}
