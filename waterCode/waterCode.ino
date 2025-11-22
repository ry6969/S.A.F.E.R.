#define PUMP_RELAY_PIN 2
#define SERVO2_PIN 6

#include <Servo.h>

Servo servo2;

void setup() {
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  digitalWrite(PUMP_RELAY_PIN, HIGH); // relay OFF initially (assuming LOW = ON)
  servo2.attach(SERVO2_PIN);
  servo2.write(90); // reset
  Serial.begin(9600);
  Serial.println("Enter 1 to turn ON the pump, 0 to turn OFF:");
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();

    if (input == '1') {
      digitalWrite(PUMP_RELAY_PIN, LOW); // Turn ON relay
      Serial.println("Pump ON");
    } else if (input == '0') {
      digitalWrite(PUMP_RELAY_PIN, HIGH); // Turn OFF relay
      Serial.println("Pump OFF");
    } else {
      Serial.println("Invalid input. Enter 1 or 0.");
    }

    // Clear any leftover characters in the buffer
    while (Serial.available() > 0) Serial.read();
  }
}
