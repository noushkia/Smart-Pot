#include <Arduino.h>

#define MAX_MOTOR_SPEED 255
#define BLUETOOTH_END_CHARACTER '/'
#define BLUETOOTH_BAUD_RATE 9600
#define DELIMITER '#'
#define EN_PIN 8
#define MOTOR_PIN 9

String buffer = "";

int getData() {
  while(true) {
      if(!Serial.available()) {
          continue;
      }

      char temp = (char) Serial.read();
      if (temp == DELIMITER) {
          float data = buffer.toInt();
          buffer = "";
          return data;
      }
      buffer += temp;
  }
}

void setup() {
  pinMode(EN_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  Serial.begin(BLUETOOTH_BAUD_RATE);
  delay(500);
}

void loop() {
  int dutyCycle = getData();
  float wateringRate = (dutyCycle / 100.0) * MAX_MOTOR_SPEED;
  digitalWrite(EN_PIN, HIGH);
  analogWrite(MOTOR_PIN, wateringRate);
}