#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define RS_LCD 12
#define EN_LCD 11
#define D4_LCD 5
#define D5_LCD 4  
#define D6_LCD 3 
#define D7_LCD 2
#define BLUETOOTH_BAUD_RATE 9600
#define DELIMITER '#'

typedef struct
{
    float humidity;
    float temperature;
} SensorData;

typedef struct
{
    SensorData sensorData;
    int ductCycle;
} LCDData;

LiquidCrystal lcd(RS_LCD, EN_LCD, D4_LCD, D5_LCD, D6_LCD, D7_LCD);
String buffer = "";

float getData() {
    while(true) {
        if(!Serial.available()) {
            continue;
        }

        char temp = (char) Serial.read();
        if (temp == DELIMITER) {
            float data = buffer.toFloat();
            buffer = "";
            return data;
        }
        buffer += temp;
    }
}

SensorData getSensorData() {
    SensorData data;
    data.humidity = getData();
    data.temperature = getData();
    return data;
}

int selectDutyCycle(SensorData sensorData) {
    double humidity = sensorData.humidity;
    double temperature = sensorData.temperature;

    if (humidity > 30) {
        return 0;
    }

    else if (humidity <= 30 && humidity > 20) {
        if (temperature < 25)
        {
            return 0;
        }
        else {
            return 10;
        }
    }

    else if (humidity <= 20 && humidity > 10) {
        return 20;
    }

    else if (humidity <= 10) {
        return 25;
    }

    else {
        return 0;
    }
}

void sendDecision(int dutyCycle) {
    Serial.print(dutyCycle);
    Serial.print(DELIMITER);
}

LCDData handleMessage() {
    SensorData sensorData = getSensorData();
    int dutyCycle = selectDutyCycle(sensorData);
    sendDecision(dutyCycle);
    return LCDData{sensorData, dutyCycle};
}

void printOnLCD(LCDData lcdData) {
    lcd.clear();
    lcd.print("T: " + String(lcdData.sensorData.temperature));
    lcd.setCursor(0, 1);
    lcd.print("H: " + String(lcdData.sensorData.humidity));
    lcd.setCursor(0, 2);
    lcd.print("Decision: " + String(lcdData.ductCycle) + "%");
}

void setup() {
    Serial.begin(BLUETOOTH_BAUD_RATE);
    lcd.begin(20, 4);
    lcd.print("Hello World!");
    delay(500);
}

void loop() {
    LCDData data = handleMessage();
    printOnLCD(data);
}
