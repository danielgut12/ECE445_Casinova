int supplyStepPin = 39;
int supplyDirectionPin = 38;
int stepsPerRevolution = 200;
int delayT = 1000;

void setup()
{
  // Declare pins as Outputs
  pinMode(supplyStepPin, OUTPUT);
  pinMode(supplyDirectionPin, OUTPUT);
}
void loop()
{
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

  // Set motor direction clockwise
  digitalWrite(supplyDirectionPin, LOW);

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
}
