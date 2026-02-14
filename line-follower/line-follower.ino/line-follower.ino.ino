// ===== PIN DEFINITIONS =====
#define IN1 D1   // Left motor control pin
#define IN3 D5   // Right motor control pin

#define IR_LEFT   D7    // Left IR sensor
#define IR_CENTER D2    // Center IR sensor
#define IR_RIGHT  D0    // Right IR sensor

// ===== MOTOR SPEED =====
#define SPEED 255   // Max PWM (not used with ENA/ENB jumpers)

// ==================== SETUP ====================
void setup() {
  // Motor pins
  pinMode(IN1, OUTPUT);  // Left motor pin
  pinMode(IN3, OUTPUT);  // Right motor pin

  // IR sensors
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_CENTER, INPUT);
  pinMode(IR_RIGHT, INPUT);

  Serial.begin(9600);  // For debugging sensors and motor actions
  Serial.println("Line follower started (inverted logic)");
}

// ==================== LOOP ====================
void loop() {
  // Read IR sensors
  int leftSensor   = digitalRead(IR_LEFT);    // 1 = line detected, 0 = white
  int centerSensor = digitalRead(IR_CENTER);
  int rightSensor  = digitalRead(IR_RIGHT);

  // Print sensor values for debugging
  Serial.print("Sensors -> L:");
  Serial.print(leftSensor);
  Serial.print(" C:");
  Serial.print(centerSensor);
  Serial.print(" R:");
  Serial.println(rightSensor);

  // --- LINE FOLLOWER DECISION (INVERTED LOGIC) ---
  // If center sees line, go straight
  if (centerSensor == 1) {
    moveForward();        // Move both motors forward
  } 
  // If left sees line, turn left
  else if (leftSensor == 1) {
    turnLeft();           // Stop/slow left motor, right motor forward
  } 
  // If right sees line, turn right
  else if (rightSensor == 1) {
    turnRight();          // Stop/slow right motor, left motor forward
  } 
  // If all sensors are white (0), stop
  else {
    stopMotors();
  }

  delay(50);  // Small delay for stability and Serial readability
}

// ==================== MOTOR CONTROL FUNCTIONS ====================

// Move both motors forward
void moveForward() {
  digitalWrite(IN1, HIGH);   // Left motor forward
  digitalWrite(IN3, HIGH);   // Right motor forward
  Serial.println("Action: Move Forward");
}

// Move both motors backward (optional)
void moveBackward() {
  digitalWrite(IN1, LOW);    // Left motor backward
  digitalWrite(IN3, LOW);    // Right motor backward
  Serial.println("Action: Move Backward");
}

// Turn left: left motor stop, right motor forward
void turnLeft() {
  digitalWrite(IN1, LOW);    // Stop/slow left motor
  digitalWrite(IN3, HIGH);   // Right motor forward
  Serial.println("Action: Turn Left");
}

// Turn right: right motor stop, left motor forward
void turnRight() {
  digitalWrite(IN1, HIGH);   // Left motor forward
  digitalWrite(IN3, LOW);    // Stop/slow right motor
  Serial.println("Action: Turn Right");
}

// Stop both motors
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  Serial.println("Action: Stop");
}
