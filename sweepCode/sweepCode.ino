#define FLAME_PIN A1
#define GAS_PIN A0
#define SERVO1_PIN 9
#define SERVO2_PIN 6
#define BUZZER_PIN 8
#define BLUE_LED_PIN 5
#define RED_LED_PIN 4
#define ORANGE_LED_PIN 3
#define PUMP_RELAY_PIN 2
#define FLAME_WINDOW 5
#define FLAME_WINDOW 5


#include <Servo.h>

//Component Object Constructor
Servo servo1;
Servo servo2;

//Function Declaration
void blinkALL(int count, int interval);
void LedOn(char color);

int readFlameLevel();
bool isFlamePresent();
void printFlameStatus();

int readSmokeLevel();
bool isSmokePresent();
void printSmokeStatus();

int sprayAngle = 90;  // servo1 initial angle
int sprayStep = 15;   // how many degrees to move per loop toward detected fire

//Global Variable
int minAngle = 60;
int maxAngle = 120;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Device Starting");

  //Pin Set-ups
  pinMode(FLAME_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(SERVO1_PIN, OUTPUT);
  pinMode(SERVO2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(ORANGE_LED_PIN, OUTPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  digitalWrite(PUMP_RELAY_PIN, HIGH); //make sure the pump is off


  Serial.println("Set-up is complete and ready!!!");
  blinkALL(2, 250);
  LedOn('B'); //Power Indicator  
}

void loop() {

  //Debug Status
  printFlameStatus();
  printSmokeStatus();

  //Scanning Motion - Servo1
  static int currentAngle = minAngle;
  int angleDifference = 10;
  static bool reachedMax = false; 
  static bool wasFlameActive = false;
  static bool wasSmokeActive = false;


  if (!isFlamePresent() && !isSmokePresent()) {
    if (reachedMax == false) {
      currentAngle += angleDifference;
      if (currentAngle >= maxAngle) {
        currentAngle = maxAngle;
        reachedMax = true;
      }
    } else if (reachedMax == true) {
      currentAngle -= angleDifference;
      if (currentAngle <= minAngle) {
        currentAngle = minAngle;
        reachedMax = false;
      }
    }

    servo1.write(currentAngle);
  }

  if (isFlamePresent()) {
    LedOn('R');

    // Servo1 points directly at the flame
    servo1.write(currentAngle);

    // Servo2 (sprayer) sweeps ±15° around servo1 angle
    int sprayRange = 20;
    int sprayMin = currentAngle - sprayRange;
    int sprayMax = currentAngle + sprayRange;

    unsigned long startTime = millis();
    bool increasing = true;
    int sprayAngle = sprayMin;

    while (millis() - startTime < 5000) { // spray for 5 seconds
        servo2.write(sprayAngle);
        digitalWrite(PUMP_RELAY_PIN, LOW); // pump ON

        // move back and forth
        if (increasing) {
            sprayAngle += 1;
            if (sprayAngle >= sprayMax) increasing = false;
        } else {
            sprayAngle -= 1;
            if (sprayAngle <= sprayMin) increasing = true;
        }

        delay(10); // adjust speed of sweep
    }

    // Stop pump and reset servos
    servo2.write(90); // center position
    digitalWrite(PUMP_RELAY_PIN, HIGH); // turn off sprayer
} else {
    LedOff('R');
    servo2.write(90); // reset sprayer
    digitalWrite(PUMP_RELAY_PIN, HIGH);
}


  static int servo2Angle = 0;
  static bool servo2Increasing = true;

  if (isSmokePresent()) {
      LedOn('O');
      
      // Sweep servo2 back and forth
      if (servo2Increasing) {
          servo2Angle += 5;
          if (servo2Angle >= 180) {
              servo2Angle = 180;
              servo2Increasing = false;
          }
      } else {
          servo2Angle -= 5;
          if (servo2Angle <= 0) {
              servo2Angle = 0;
              servo2Increasing = true;
          }
      }
      
      servo2.write(servo2Angle);
      digitalWrite(PUMP_RELAY_PIN, LOW); // Turn on motor here (non-blocking)
      wasSmokeActive = true;

  } else {
      LedOff('O');
      servo2.write(90);
      if (wasSmokeActive == true){
        delay(500);
      }
      digitalWrite(PUMP_RELAY_PIN, HIGH); // Turn off motor here
      wasSmokeActive = false;
  }


  delay (80);

}

void blinkALL(int count, int interval){
  for (int i = 1; i <= count; i++){
    digitalWrite(BLUE_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(ORANGE_LED_PIN, HIGH);

    delay(interval);

    digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(ORANGE_LED_PIN, LOW);

    delay(interval);
  }
}

void LedOn(char color){
  if (color == 'B'){
    digitalWrite(BLUE_LED_PIN, HIGH);
  } else if (color == 'R'){
    digitalWrite(RED_LED_PIN, HIGH);
  } else if (color == 'O') {
    digitalWrite(ORANGE_LED_PIN, HIGH);
  }
}

void LedOff(char color){
  if (color == 'B'){
    digitalWrite(BLUE_LED_PIN, LOW);
  } else if (color == 'R'){
    digitalWrite(RED_LED_PIN, LOW);
  } else if (color == 'O') {
    digitalWrite(ORANGE_LED_PIN, LOW);
  }
}

int readFlameLevel(){
  int flameLevel = analogRead(FLAME_PIN);
  return flameLevel;
}

bool isFlamePresent(){
  if (readFlameLevel() < 800){
    Serial.print("FLAME DETECTED! ");
    return true;
  } else return false;
}

void printFlameStatus(){
  Serial.print("Flame Value: ");
  Serial.print(readFlameLevel());
  if (isFlamePresent()){
    Serial.println(" Status: Flame Dectected!!!");
  } else {
    Serial.println(" Status: No Flame Detected");
  }
  
}

int readSmokeLevel(){
  int smokeLevel = analogRead(GAS_PIN);
  return smokeLevel;
}

bool isSmokePresent(){
  if (readSmokeLevel() > 370){
    return true;
  } else return false;
}

void printSmokeStatus(){
  Serial.print("Smoke Value: ");
  Serial.print(readSmokeLevel());

  if (isSmokePresent()){
    Serial.println(" Status: Smoke Dectected!!!");
  } else {
    Serial.println(" Status: No Smoke Detected");
  }
}