// Motor A connections
int enA = 37;
int in1 = 38;
int in2 = 39;
// Motor B connections
//int enB = 3;
//int in3 = 5;
//int in4 = 4;


// TOF Sensor Information
int RXD2 = 42;
int TXD2 = 41;
String distance = "";  // Buffer to store the full reading
float dist_flt = 0.0;
int motor_power = 0;
int scaling_factor = 1.33;

// Switch Information
int switchPin = 40;
int switchState = 0;

void setup() {
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
//  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
//  pinMode(in3, OUTPUT);
//  pinMode(in4, OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, LOW);

  // TOF setup
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("TOF10120 Real-Time Distance Readings...");

  // Switch Setup
  pinMode(switchPin, INPUT); // Set the pin as an input
}

void loop() {
  // Motor Control Testing
//  directionControl();
//  delay(1000);
//  speedControl();
//  delay(1000);

  // TOF Input
  while (Serial2.available()) {
      char c = Serial2.read();

      if (c == '\n' || c == '\r') {  
          // Newline or carriage return means the number is complete
          if (distance.length() > 0) {
              Serial.print("Distance: ");
              Serial.print(distance + " mm     ");
              dist_flt = atof(distance.c_str()) * scaling_factor; // Convert to float
              motor_power = map(dist_flt, 0, 2000, 50, 175); // Map distance to 0-255 range
              Serial.print("Motor Power: ");
              Serial.println(motor_power);

              distance = "";  // Reset buffer for next reading
          }
      } else if (isDigit(c)) {  
          // Append only numeric characters to build the distance value
          distance += c;

//          Serial.print(dist_flt);
      }
  }

  // Switch Logic
  switchState = digitalRead(switchPin); // Read the state of the switch
  if (switchState == HIGH) {
    Serial.println("Switch is activated.");
    int final_motor_power = motor_power;
    directionControl(final_motor_power);
  } else {
    Serial.println("Switch is not activated.");
  }

  delay(100); // Optional delay for serial output clarity
}

// This function lets you control spinning direction of motors
void directionControl(int motor_power) {
  while (switchState == HIGH) {
    Serial.print("MOTOR ON...       ");
    Serial.println(motor_power);
    // Set motors to maximum speed
    // For PWM maximum possible values are 0 to 255
    analogWrite(enA, motor_power);
    //  analogWrite(enB, motor_power);
    
    // Turn on motor A & B
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    //  digitalWrite(in3, HIGH);
    //  digitalWrite(in4, LOW);
    delay(1000);
    
    // Now change motor directions
    //  digitalWrite(in1, LOW);
    //  digitalWrite(in2, HIGH);
    //  digitalWrite(in3, LOW);
    //  digitalWrite(in4, HIGH);
    //  delay(2000);
    switchState = digitalRead(switchPin); // Read the state of the switch
  }
  
  Serial.println("MOTOR OFF...");
  // Turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, LOW);
}

// This function lets you control speed of the motors
void speedControl() {
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    analogWrite(enA, i);
//    analogWrite(enB, i);
    delay(20);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i) {
    analogWrite(enA, i);
//    analogWrite(enB, i);
    delay(20);
  }
  
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
//  digitalWrite(in3, LOW);
//  digitalWrite(in4, LOW);
}
