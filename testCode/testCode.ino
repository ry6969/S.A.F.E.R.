#define FLAME_PIN A1
#define GAS_PIN A0
#define BUZZER_PIN 8
#define BLUE_LED_PIN 5
#define RED_LED_PIN 4
#define ORANGE_LED_PIN 3

//Function Declaration
void bootDisplay();
void blinkALL(int count, int interval);
void LedOn(char color);
void LedOFff(char color);
void buzzerAlert(char type);

int readFlameLevel();
bool isFlamePresent();
bool calculateFlicker();
void printFlameStatus();

int readSmokeLevel();
bool isSmokePresent();
void printSmokeStatus();

void setup() {

  Serial.begin(9600);
  Serial.println("Device Starting");

  //Pin Set-ups
  pinMode(FLAME_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(ORANGE_LED_PIN, OUTPUT);

  bootDisplay();
  blinkALL(2, 250);
  Serial.println("Set-up is complete and ready!!!");
  LedOn('B');

}

//Global variable
unsigned long lastBeepTime = 0;
bool buzzerState = false;

void loop() {
  printFlameStatus();

  unsigned long currentMillis = millis();
  unsigned long interval = 0;

  // Fire has priority over smoke
  if (isFlamePresent()) {
    LedOn('R');
    LedOff('O');
    interval = 100; // fast beep
  } else if (isSmokePresent()) {
    LedOn('O');
    LedOff('R');
    interval = 250; // slow beep
  } else {
    // nothing detected, turn off
    LedOff('R');
    LedOff('O');
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }

  // Non-blocking buzzer toggle
  if (currentMillis - lastBeepTime >= interval) {
    lastBeepTime = currentMillis;
    buzzerState = !buzzerState;
    digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
  }

  delay(50); // loop delay
}


void bootDisplay(){
  LedOn('O');
  delay(100);
  LedOn('R');
  delay(100);
  LedOn('B');
  delay(100);
  LedOff('O');
  LedOff('R');
  LedOff('B');
}

void blinkALL(int count, int interval){
  for (int i = 1; i <= count+1; i++){
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


void buzzerAlert(char type) {
  static unsigned long lastBeepSmoke = 0;
  static unsigned long lastBeepFire  = 0;
  static bool buzzerStateSmoke = false;
  static bool buzzerStateFire  = false;
  unsigned long currentMillis = millis();

  if(type=='S'){
    if(currentMillis - lastBeepSmoke >= 250){ // slow beep
      lastBeepSmoke = currentMillis;
      buzzerStateSmoke = !buzzerStateSmoke;
      digitalWrite(BUZZER_PIN, buzzerStateSmoke ? HIGH : LOW);
    }
  } else if(type=='F'){
    if(currentMillis - lastBeepFire >= 100){ // fast beep
      lastBeepFire = currentMillis;
      buzzerStateFire = !buzzerStateFire;
      digitalWrite(BUZZER_PIN, buzzerStateFire ? HIGH : LOW);
    }
  }
}


int readFlameLevel(){
  int flameLevel = analogRead(FLAME_PIN);
  return flameLevel;
}

bool isFlamePresent(){
  if (readFlameLevel() < 1000){
    if(calculateFlicker()){
      return true;
    } else return false;
  } else return false;
}

bool calculateFlicker() {
    const int samples = 15;                   // number of samples
    const float threshold = 4;          // adjust this based on testing
    int readings[samples];

    // 1. Collect samples
    for (int i = 0; i < samples; i++) {
        readings[i] = analogRead(FLAME_PIN);
        delay(3);                      // helps capture flicker; edit as needed
    }

    //Compute mean
    float sum = 0;
    for (int i = 0; i < samples; i++) sum += readings[i];
    float mean = sum / samples;

    //Compute variance
    float varSum = 0;
    for (int i = 0; i < samples; i++) {
        float diff = readings[i] - mean;
        varSum += diff * diff;
    }
    float variance = varSum / samples;

    //Standard Deviation
    float sd = sqrt(variance);

    //Serial.print("SD: "); Serial.println(sd);

    //Decision
    return (sd > threshold);
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
  if (readSmokeLevel() > 280){
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
