#define FLAME_PIN A1
#define GAS_PIN A0
#define SERVO1_PIN 9
#define SERVO2_PIN 6
#define BUZZER_PIN 8
#define BLUE_LED_PIN 5
#define RED_LED_PIN 4
#define ORANGE_LED_PIN 3
#define PUMP_RELAY_PIN 2

void blinkALL(int count, int interval);
void LedOn(char color);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Device Starting");

  pinMode(FLAME_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(SERVO1_PIN, OUTPUT);
  pinMode(SERVO2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(ORANGE_LED_PIN, OUTPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  Serial.println("Set-up is complete and ready!!!");
  blinkALL(2, 250);
  LedOn('B'); //Power Indicator

  
}

void loop() {
  // put your main code here, to run repeatedly:

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
