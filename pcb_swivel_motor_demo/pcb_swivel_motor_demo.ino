int swivelStepPin = 5;
int swivelDirectionPin = 4;
int stepsPerRevolution = 200;
int delayT = 1000;

void setup()
{
  // Declare pins as Outputs
  pinMode(swivelStepPin, OUTPUT);
  pinMode(swivelDirectionPin, OUTPUT);
}
void loop()
{
  // Set motor direction clockwise
  digitalWrite(swivelDirectionPin, HIGH);

  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(swivelStepPin, HIGH);
    delayMicroseconds(4000);
    digitalWrite(swivelStepPin, LOW);
    delayMicroseconds(4000);
  }

  // Wait a second
  delay(delayT);

  // Set motor direction clockwise
  digitalWrite(swivelDirectionPin, LOW);

  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(swivelStepPin, HIGH);
    delayMicroseconds(4000);
    digitalWrite(swivelStepPin, LOW);
    delayMicroseconds(4000);
  }

  // Wait a second
  delay(delayT);
}
