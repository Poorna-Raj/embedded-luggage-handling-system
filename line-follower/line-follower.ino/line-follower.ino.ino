
#define IN1 D1 // Left motor forward
#define IN3 D4 // Right motor forward

#define ENA D5 // Left motor PWM speed
#define ENB D6 // Right motor PWM speed

#define IR_LEFT D7
#define IR_CENTER D2
#define IR_RIGHT D0

// ===== MOTOR SPEED =====
#define FORWARD_SPEED 150
#define TURN_SPEED 100
#define PIVOT_SPEED 230

// ==================== SETUP ====================
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_CENTER, INPUT);
  pinMode(IR_RIGHT, INPUT);

  Serial.begin(9600);
  Serial.println("Line follower started (3 sensor normal logic)");
}

// ==================== LOOP ====================
void loop() {

  int L = digitalRead(IR_LEFT);
  int C = digitalRead(IR_CENTER);
  int R = digitalRead(IR_RIGHT);

  Serial.print("L:"); Serial.print(L);
  Serial.print(" C:"); Serial.print(C);
  Serial.print(" R:"); Serial.print(R);
  Serial.print("  →  ");

  // ===== NORMAL LOGIC (1 = BLACK LINE) =====

  // Perfect center
  if (L == 0 && C == 1 && R == 0) {
    moveForward();
  }

  // Slight right drift (right sensor on line)
  else if (L == 0 && C == 1 && R == 1) {
    gentleRight();
  }

  // Slight left drift (left sensor on line)
  else if (L == 1 && C == 1 && R == 0) {
    gentleLeft();
  }

  // Sharp right turn
  else if (L == 0 && C == 0 && R == 1) {
    pivotRight();
  }

  // Sharp left turn
  else if (L == 1 && C == 0 && R == 0) {
    pivotLeft();
  }

  // All black (intersection)
  else if (L == 1 && C == 1 && R == 1) {
    moveForward();
  }

  // Lost line
  else {
    stopMotors();
  }

  delay(10);
}

// ==================== MOTOR FUNCTIONS ====================

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  analogWrite(ENA, FORWARD_SPEED);
  analogWrite(ENB, FORWARD_SPEED);
  Serial.println("FORWARD");
}

void gentleRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  analogWrite(ENA, FORWARD_SPEED);
  analogWrite(ENB, TURN_SPEED);
  Serial.println("GENTLE RIGHT");
}

void gentleLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, FORWARD_SPEED);
  Serial.println("GENTLE LEFT");
}

void pivotRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, LOW);
  analogWrite(ENA, PIVOT_SPEED);
  analogWrite(ENB, 0);
  Serial.println("PIVOT RIGHT");
}

void pivotLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, HIGH);
  analogWrite(ENA, 0);
  analogWrite(ENB, PIVOT_SPEED);
  Serial.println("PIVOT LEFT");
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  Serial.println("STOP");
}
