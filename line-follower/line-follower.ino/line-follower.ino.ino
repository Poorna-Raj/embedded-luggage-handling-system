#include <ESP8266WiFi.h>

// ===== WIFI SETTINGS =====
const char* ssid = "SLT-Fiber-2.4G-0A6C";
const char* password = "873399703%";

const char* serverIP = "192.168.1.5";   // CHANGE THIS
const uint16_t serverPort = 5000;        // CHANGE THIS

WiFiClient client;

// ===== MOTOR PINS =====
#define IN1 D1
#define IN3 D4

#define ENA D5
#define ENB D6

// ===== IR SENSORS =====
#define IR_LEFT   D7
#define IR_CENTER D2
#define IR_RIGHT  D0

// ===== BUTTON =====
#define DONE_BUTTON D3   // connect push button to D3 and GND

// ===== MOTOR SPEED =====
#define FORWARD_SPEED 150
#define TURN_SPEED 100
#define PIVOT_SPEED 230

// ==================== SETUP ====================
void setup() {

  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_CENTER, INPUT);
  pinMode(IR_RIGHT, INPUT);

  pinMode(DONE_BUTTON, INPUT_PULLUP);  // button to GND

  // ===== CONNECT WIFI =====
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // ===== CONNECT TO TCP SERVER =====
  connectToServer();
}

// ==================== LOOP ====================
void loop() {

  // Reconnect if disconnected
  if (!client.connected()) {
    connectToServer();
  }

  // ===== RECEIVE DATA FROM SERVER =====
  if (client.available()) {
    String message = client.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(message);
  }

  // ===== BUTTON SEND DONE =====
  if (digitalRead(DONE_BUTTON) == LOW) {
    client.println("DONE");
    Serial.println("Sent: DONE");
    delay(500);  // debounce delay
  }

  // ===== LINE FOLLOWER =====
  int L = digitalRead(IR_LEFT);
  int C = digitalRead(IR_CENTER);
  int R = digitalRead(IR_RIGHT);

  if (L == 0 && C == 1 && R == 0) {
    moveForward();
  }
  else if (L == 0 && C == 1 && R == 1) {
    gentleRight();
  }
  else if (L == 1 && C == 1 && R == 0) {
    gentleLeft();
  }
  else if (L == 0 && C == 0 && R == 1) {
    pivotRight();
  }
  else if (L == 1 && C == 0 && R == 0) {
    pivotLeft();
  }
  else if (L == 1 && C == 1 && R == 1) {
    moveForward();
  }
  else {
    stopMotors();
  }

  delay(10);
}

// ==================== WIFI CONNECT FUNCTION ====================
void connectToServer() {

  Serial.println("Connecting to TCP server...");

  while (!client.connect(serverIP, serverPort)) {
    Serial.println("Connection failed. Retrying...");
    delay(2000);
  }

  Serial.println("Connected to TCP server!");
}

// ==================== MOTOR FUNCTIONS ====================
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  analogWrite(ENA, FORWARD_SPEED);
  analogWrite(ENB, FORWARD_SPEED);
}

void gentleRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  analogWrite(ENA, FORWARD_SPEED);
  analogWrite(ENB, TURN_SPEED);
}

void gentleLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, FORWARD_SPEED);
}

void pivotRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, LOW);
  analogWrite(ENA, PIVOT_SPEED);
  analogWrite(ENB, 0);
}

void pivotLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, HIGH);
  analogWrite(ENA, 0);
  analogWrite(ENB, PIVOT_SPEED);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
