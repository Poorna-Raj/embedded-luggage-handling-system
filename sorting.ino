#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ============= COUNTERS =================
int redCount = 0;
int blueCount = 0;
int greenCount = 0;

// ============= LCD Displays =============
LiquidCrystal_I2C lcd(0x27,16,2);

// ============= GEAR MOTOR ===============
#define MOTOR_SPEED_PIN 25  // D25 
#define MOTOR_DIR_PIN 26    // D26 
#define SET_SPEED 100      

bool isConveyorRunning = false;

// ============= IR SENSOR ================
#define MAIN_IR 33 // D33
const int DEBOUNCE_DELAY = 50;

// ============= COLOR SENSOR =============
#define S2_PIN 18  // D18
#define S3_PIN 19  // D19
#define OUT_PIN 34 // D34

// ============= COLOR ENUM ===============
enum Color {
  RED,
  GREEN,
  BLUE,
  UNKNOWN
};

// ============= SERVO ====================
Servo servo;
#define SERVO_PIN 27 // D27

void setup() {
  Serial.begin(115200);
  
  lcd.init();
  lcd.backlight();

  pinMode(MOTOR_SPEED_PIN, OUTPUT);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  pinMode(MAIN_IR, INPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  pinMode(OUT_PIN, INPUT);

  servo.attach(SERVO_PIN,500,2400);
  servo.write(45);

  startConveyor();
}

void loop() {
  if (isConveyorRunning && isIrDetected(MAIN_IR)) {
    stopConveyor();
    
    Serial.println("--- Reading Color ---");
    delay(500);
    Color detectedColor = readColor();
    printColorName(detectedColor);

    sortObjects(detectedColor); 
    updateLcdDisplay();
    startConveyor();
  }
}

Color readColor() {
  int redFreq, greenFreq, blueFreq;

  // Read RED
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, LOW);
  redFreq = pulseIn(OUT_PIN, LOW);
  delay(50);

  // Read GREEN
  digitalWrite(S2_PIN, HIGH);
  digitalWrite(S3_PIN, HIGH);
  greenFreq = pulseIn(OUT_PIN, LOW);
  delay(50);

  // Read BLUE
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, HIGH);
  blueFreq = pulseIn(OUT_PIN, LOW);
  delay(50);

  if (redFreq < greenFreq && redFreq < blueFreq) return RED;
  if (greenFreq < redFreq && greenFreq < blueFreq) return GREEN;
  if (blueFreq < redFreq && blueFreq < greenFreq) return BLUE;
  
  return UNKNOWN;
}

// Helper to print Enum values
void printColorName(Color c) {
  Serial.print("Detected Color: ");
  switch(c) {
    case RED:     Serial.println("RED");     break;
    case GREEN:   Serial.println("GREEN");   break;
    case BLUE:    Serial.println("BLUE");    break;
    case UNKNOWN: Serial.println("UNKNOWN"); break;
  }
}

void startConveyor() {
  digitalWrite(MOTOR_DIR_PIN, HIGH);
  analogWrite(MOTOR_SPEED_PIN, SET_SPEED);
  isConveyorRunning = true; 
  Serial.println("Conveyor Started!");
}

void stopConveyor() {
  analogWrite(MOTOR_SPEED_PIN, 0); 
  digitalWrite(MOTOR_DIR_PIN, LOW);
  isConveyorRunning = false;
  Serial.println("Conveyor Stopped!");
}

bool isIrDetected(const int pin) {
  if (!digitalRead(pin)) { 
    delay(DEBOUNCE_DELAY);
    if (!digitalRead(pin)) return true;
  }
  return false;
}

void sortObjects(Color c) {
  switch(c) {
    case RED:
      Serial.println("Sorting to RED bin...");
      servo.write(90);
      startConveyor();
      delay(2000);
      stopConveyor();
      servo.write(45);
      redCount++;
      break;
    case BLUE:
      Serial.println("Sorting to BLUE bin...");
      servo.write(180);
      startConveyor();
      delay(3000);
      stopConveyor();
      servo.write(45);
      blueCount++;
      break;
    case GREEN:
      Serial.println("Sorting to GREEN bin...");
      greenCount++;
      break;
    default:
      Serial.println("Unknown Color");
      break;
  }
}

void updateLcdDisplay() {
  lcd.setCursor(0,0);
  lcd.print("R=");
  lcd.print(redCount);
  lcd.print("   ");

  lcd.setCursor(5,0);
  lcd.print("B=");
  lcd.print(blueCount);
  lcd.print("   ");

  lcd.setCursor(10,0);
  lcd.print("G=");
  lcd.print(greenCount);
  lcd.print("   ");
}

