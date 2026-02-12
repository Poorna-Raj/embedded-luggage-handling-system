// ============= GEAR MOTOR ===============
#define MOTOR_SPEED_PIN 5  // D1 
#define MOTOR_DIR_PIN 4    // D2 
#define SET_SPEED 100      

bool isConveyorRunning = false;

// ============= IR SENSOR ================
#define MAIN_IR 16 // D0
const int DEBOUNCE_DELAY = 50;

// ============= COLOR SENSOR =============
#define S2_PIN 14  // D5
#define S3_PIN 12  // D6
#define OUT_PIN 13 // D7

// ============= COLOR ENUM ===============
enum Color {
  RED,
  GREEN,
  BLUE,
  UNKNOWN
};

void setup() {
  Serial.begin(9600);
  
  pinMode(MOTOR_SPEED_PIN, OUTPUT);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  pinMode(MAIN_IR, INPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  pinMode(OUT_PIN, INPUT);

  startConveyor();
}

void loop() {
  if (isConveyorRunning && isIrDetected(MAIN_IR)) {
    stopConveyor();
    
    Serial.println("--- Reading Color ---");
    Color detectedColor = readColor();
    
    // Using the enum in logic
    printColorName(detectedColor);
    
    delay(2000); 
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