#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define RS_LCD 12
#define EN_LCD 11
#define D4_LCD 5
#define D5_LCD 4  
#define D6_LCD 3 
#define D7_LCD 2
#define BLUETOOTH_DELIMETER_CHARACTER '-'
#define BLUETOOTH_END_CHARACTER '/'
#define BLUETOOTH_BAUD_RATE 9600

typedef struct
{
    double humidity;
    double temperature;
} SensorData;

typedef struct
{
  SensorData SensorData;
  int ductCycle;
} LCDData;

String msgBuffer = "";

LiquidCrystal lcd(RS_LCD, EN_LCD, D4_LCD, D5_LCD, D6_LCD, D7_LCD);

SensorData getSensorData(String sensorMessage) {
    int delimeterIndex = sensorMessage.indexOf(BLUETOOTH_DELIMETER_CHARACTER);
    int endIndex = sensorMessage.indexOf(BLUETOOTH_END_CHARACTER);
    String lastHumidityValueStr = sensorMessage.substring(0, delimeterIndex);
    String lastTempValueStr = sensorMessage.substring(delimeterIndex + 1, endIndex);
    return SensorData{atof(&lastHumidityValueStr[0]), atof(&lastTempValueStr[0])};
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
    String decision = String(dutyCycle) + BLUETOOTH_END_CHARACTER;
    Serial.write(&decision[0]);
}

LCDData handleMessage(String message) {
    SensorData sensorData = getSensorData(message);
    int dutyCycle = selectDutyCycle(sensorData);
    sendDecision(dutyCycle);
    return LCDData{sensorData, dutyCycle};
}

void printOnLCD(LCDData lcdData) {
    lcd.clear();
    
    lcd.setCursor(0, 1);
    lcd.print("Temperature: " + String(lcdData.SensorData.temperature));

    lcd.setCursor(0, 2);
    lcd.print("Humidity: " + String(lcdData.SensorData.humidity));
    
    lcd.setCursor(0, 3);
    lcd.print("Decision(Duty Cycle): " + String(lcdData.ductCycle) + "%");
}

void setup() {
  Serial.begin(BLUETOOTH_BAUD_RATE);
  lcd.begin(20, 4);
  lcd.print("Hello World!");
}

void loop() {
  String msg = Serial.readString();
  if (msg != "") {
      msgBuffer += msg;
      // if end of Bluetooth message is reached
      if (msgBuffer.indexOf(BLUETOOTH_END_CHARACTER) != -1) 
      {
          LCDData lcdData = handleMessage(msgBuffer);
          printOnLCD(lcdData);
          msgBuffer = "";
      }
      
  }
}