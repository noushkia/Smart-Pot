#include <Wire.h>
#include <LiquidCrystal.h>
#include <Arduino.h>

#define RS_LCD 9
#define EN_LCD 8 
#define D4_LCD 7 
#define D5_LCD 6  
#define D6_LCD 5 
#define D7_LCD 4
#define BLUETOOTH_DELIMETER_CHARACTER '-'
#define BLUETOOTH_END_CHARACTER '/'
#define BLUETOOTH_BAUD_RATE 9600
#define MAX_PWM_SPEED 256

LiquidCrystal lcd(RS_LCD, EN_LCD, D4_LCD, D5_LCD, D6_LCD, D7_LCD);

String msgBuffer = "";

void setup() {
    Serial.begin(BLUETOOTH_BAUD_RATE); // todo: how to connect the send channel to a module and the write to another?
    Serial.setTimeout(0);
    lcd.begin(20, 4);
}

void handleMessage() {
    // fetch temp and humidity
    // decide the duty cycle
    // send the decision to actuator
}

void printOnLCD() {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Hello World!");
    // lcd.print("Temperature: " + String(cTemp_value));
    // lcd.setCursor(0, 2);
    // lcd.print("Humidity: " + String(humidity_value));
    // lcd.setCursor(0, 3);
    // lcd.print("Decision(Duty Cycle): " + String(decision));
}

void loop() {
    String msg = Serial.readString();
    if (msg != "") {
        msgBuffer += msg;
        // if end of Bluetooth message is reached
        if (msgBuffer.indexOf(BLUETOOTH_END_CHARACTER) != -1) 
        {
            handleMessage();
            printOnLCD();
            msgBuffer = "";
        }
        
    }
}
