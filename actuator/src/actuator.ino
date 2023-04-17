#include <Wire.h>
#include <Arduino.h>

#define BLUETOOTH_DELIMETER_CHARACTER '-'
#define BLUETOOTH_END_CHARACTER '/'
#define BLUETOOTH_BAUD_RATE 9600
#define MOTOR_OUT 11

int currPWM = 0;
String msgBuffer = "";

void setup() {
    Wire.begin();
    pinMode(MOTOR_OUT, OUTPUT);
    Serial.begin(BLUETOOTH_BAUD_RATE);
    Serial.setTimeout(0);
}

void setMotorPWM() {
    String dutyCycle = msgBuffer.substring(0, msgBuffer.length() - 1);
    currPWM = atoi(&dutyCycle[0]);
}

void loop() {
    analogWrite(MOTOR_OUT, currPWM);
    // Do we need message buffer for actuator as well?
    String msg = Serial.readString();
    if (msg != ""){
        msgBuffer += msg;
        if(msg.indexOf(BLUETOOTH_END_CHARACTER) != -1){
            setMotorPWM();
        }
        msgBuffer = "";
    }
}