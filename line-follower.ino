// =========== Motor ==============
#define ENA D5
#define IN1 D1
#define IN2 D2

#define ENB D6
#define IN3 D7
#define IN4 D8

// =========== IR Sensors =========
#define IR_LEFT D4
#define IR_CENTER D4
#define IR_RIGHT D4

// =========== Speed ==============
#define BASE_SPEED 180
#define TURN_SPEED 150

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_CENTER, INPUT);
  pinMode(IR_RIGHT, INPUT);
}

void loop() {
  followLine();
}

void followLine() {
  bool left = digitalRead(IR_LEFT);
  bool center = digitalRead(IR_CENTER);
  bool right = digitalRead(IR_RIGHT);

  if(!center) {
    moveForward(BASE_SPEED, BASE_SPEED);
  }
  else if(!left) {
    moveForward(TURN_SPEED, BASE_SPEED);
  }
  else if(!right) {
    moveForward(BASE_SPEED, TURN_SPEED);
  }
  else {
    stopMotors();
  }
}

void moveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}