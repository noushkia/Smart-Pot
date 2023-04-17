#include <Wire.h>
#include <LiquidCrystal.h>
#include <Arduino.h>

#define RS_LCD 9
#define EN_LCD 8 
#define D4_LCD 7 
#define D5_LCD 6  
#define D6_LCD 5 
#define D7_LCD 4
#define BLUETOOTH_DELIMITER_CHARACTER '-'
#define BLUETOOTH_END_CHARACTER '/'
#define BLUETOOTH_BAUD_RATE 9600
#define MAX_PWM_SPEED 256


typedef struct
{
    double humidity;
    double temperature;
} SensorData;


LiquidCrystal lcd(RS_LCD, EN_LCD, D4_LCD, D5_LCD, D6_LCD, D7_LCD);


void setup() {
    Serial.begin(BLUETOOTH_BAUD_RATE); // todo: how to connect the send channel to a module and the write to another?
    Serial.setTimeout(0);
    lcd.begin(20, 4);
}

SensorData getSensorData(String sensorMessage) {
    int delimiterIndex = sensorMessage.indexOf(BLUETOOTH_DELIMITER_CHARACTER);
    int endIndex = sensorMessage.indexOf(BLUETOOTH_END_CHARACTER);
    // issue with temperature and humidity conversion
    // reading the data in a wrong order
    String lastHumidityValueStr = sensorMessage.substring(0, delimiterIndex);
    String lastTempValueStr = sensorMessage.substring(delimiterIndex + 1, endIndex);
    return SensorData{atof(&lastHumidityValueStr[0]), atof(&lastTempValueStr[0])};
}

int selectDutyCycle(SensorData sensorData) {
    double humidity = sensorData.humidity;
    double temperature = sensorData.temperature;

    if (humidity > 30) {
        return 0;
    }
    if (humidity <= 30 && humidity > 20) {
        if (temperature < 25)
        {
            return 0;
        }
        else {
            return 10;
        }
    }
    if (humidity <= 20 && humidity > 10) {
        return 20;
    }
    if (humidity <= 10) {
        return 25;
    }
    return 0;
}

void sendDecision(int dutyCycle) {
    String decision = String(dutyCycle) + BLUETOOTH_END_CHARACTER;
    Serial.write(&decision[0]);
}

void handleMessage(String message) {
    SensorData sensorData = getSensorData(message);
    int dutyCycle = selectDutyCycle(sensorData);
    sendDecision(dutyCycle);
    delay(10);
    printOnLCD(sensorData, dutyCycle);
}

void printOnLCD(SensorData sensorData, int dutyCycle) {
    double humidity = sensorData.humidity;
    double temperature = sensorData.temperature;

    lcd.clear();
    lcd.print("T: " + String(temperature));
    lcd.print("\nH: " + String(humidity));
    lcd.print("\nD: " + String(dutyCycle));
}

void loop() {
    String msgBuffer = "";
    String msg = Serial.readString();
    if (msg != "") {
        msgBuffer += msg;
        // if end of Bluetooth message is reached
        if (msgBuffer.indexOf(BLUETOOTH_END_CHARACTER) != -1) 
        {
            handleMessage(msgBuffer);
            msgBuffer = "";
        }
    }
}

// void sendDecision(int dutyCycle) {
//     char decision[4];
//     itoa(dutyCycle, decision, 10);
//     decision[3] = BLUETOOTH_END_CHARACTER;
//     Serial.write(decision, 4);
// }
