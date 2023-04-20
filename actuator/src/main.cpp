#include <Arduino.h>

#define MAX_MOTOR_SPEED 256
#define BLUETOOTH_END_CHARACTER '/'
#define BLUETOOTH_BAUD_RATE 9600

String msgBuffer = "";

int getDutyCycle(String message) {
  int endIndex = message.indexOf(BLUETOOTH_END_CHARACTER);
  String dutyCycleStr = message.substring(0, endIndex);
  return atoi(&dutyCycleStr[0]);
}

void handleMessage(String message) {
    int dutyCycle = getDutyCycle(message);
    float wateringRate = (dutyCycle / 100.0) * MAX_MOTOR_SPEED;
    // For debugging
    Serial.println(wateringRate);
}

void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  // Serial.begin(BLUETOOTH_BAUD_RATE);
}

void loop() {
  // while(true) {
  //   if (!Serial.available()) 
  //     continue;
  //   float rate = Serial.parseFloat();
  //   Serial.println(rate);
  digitalWrite(8, HIGH);
  analogWrite(9, 255);
    // if (msg != "") {
    //     msgBuffer += msg;
    //     // if end of Bluetooth message is reached
    //     if (msgBuffer.indexOf(BLUETOOTH_END_CHARACTER) != -1) 
    //     {
    //         handleMessage(msgBuffer);
    //         msgBuffer = "";
    //     }
    // }
  // }

}