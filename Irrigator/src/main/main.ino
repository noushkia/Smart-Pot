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


typedef struct
{
    float humidity;
    float temperature;
} SensorData;


LiquidCrystal lcd(RS_LCD, EN_LCD, D4_LCD, D5_LCD, D6_LCD, D7_LCD);


void setup() {
    Serial.begin(BLUETOOTH_BAUD_RATE); // todo: how to connect the send channel to a module and the write to another?
    Serial.setTimeout(0);
    lcd.begin(20, 4);
}

SensorData getSensorData(String sensorMessage) {
    int delimeterIndex = sensorMessage.indexOf(BLUETOOTH_DELIMETER_CHARACTER);
    int endIndex = sensorMessage.indexOf(BLUETOOTH_END_CHARACTER);
    String lastHumidityValueStr = sensorMessage.substring(0, delimeterIndex);
    String lastTempValueStr = sensorMessage.substring(delimeterIndex + 1, endIndex);
    return SensorData{atof(&lastHumidityValueStr[0]), atof(&lastTempValueStr[0])};
}

int selectDutyCycle(SensorData sensorData) {
    float humidity = sensorData.humidity;
    float temperature = sensorData.temperature;

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
}

void sendDecision(int dutyCycle) {
    String decision = String(dutyCycle) + BLUETOOTH_END_CHARACTER;
    Serial.write(&decision[0]);
}

void handleMessage(String message) {
    SensorData sensorData = getSensorData(message);
    int dutyCycle = selectDutyCycle(sensorData);
    sendDecision(dutyCycle);
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
    String msgBuffer = "";
    String msg = Serial.readString();
    if (msg != "") {
        msgBuffer += msg;
        // if end of Bluetooth message is reached
        if (msgBuffer.indexOf(BLUETOOTH_END_CHARACTER) != -1) 
        {
            handleMessage(msgBuffer);
            printOnLCD();
            msgBuffer = "";
        }
        
    }
}



// The code seems to have some potential issues and areas for improvement:

// In the setup() function, there is no code to initialize the pins used for PWM output. If the duty cycle is supposed to control a motor or LED, the pins should be initialized as outputs with pinMode() function.

// The selectDutyCycle() function does not have a return statement if none of the conditions are met. This can cause undefined behavior if the function is called with an input that does not match any of the conditions. It is better to add a default return statement at the end of the function.

// The sendDecision() function sends a String object over the serial port. This can be inefficient and cause delays if the length of the String object is large. It is better to send a character array using the Serial.write() function.

// The printOnLCD() function only prints a fixed message to the LCD. It can be improved by using the sensor data and decision values to display more relevant information.

// The loop() function does not have a delay between iterations. Without a delay, the function can execute too fast and cause the microcontroller to be overloaded. It is recommended to add a delay of a few milliseconds using the delay() function.

// Here is an improved version of the code:

// #include <Wire.h>
// #include <LiquidCrystal.h>
// #include <Arduino.h>

// #define RS_LCD 9
// #define EN_LCD 8 
// #define D4_LCD 7 
// #define D5_LCD 6  
// #define D6_LCD 5 
// #define D7_LCD 4
// #define BLUETOOTH_DELIMITER_CHARACTER '-'
// #define BLUETOOTH_END_CHARACTER '/'
// #define BLUETOOTH_BAUD_RATE 9600
// #define MAX_PWM_SPEED 256
// #define PWM_PIN 10

// typedef struct
// {
//     float humidity;
//     float temperature;
// } SensorData;

// LiquidCrystal lcd(RS_LCD, EN_LCD, D4_LCD, D5_LCD, D6_LCD, D7_LCD);

// void setup() {
//     Serial.begin(BLUETOOTH_BAUD_RATE);
//     Serial.setTimeout(0);
//     pinMode(PWM_PIN, OUTPUT);
//     lcd.begin(20, 4);
// }

// SensorData getSensorData(String sensorMessage) {
//     int delimiterIndex = sensorMessage.indexOf(BLUETOOTH_DELIMITER_CHARACTER);
//     int endIndex = sensorMessage.indexOf(BLUETOOTH_END_CHARACTER);
//     String lastHumidityValueStr = sensorMessage.substring(0, delimiterIndex);
//     String lastTempValueStr = sensorMessage.substring(delimiterIndex + 1, endIndex);
//     return SensorData{atof(&lastHumidityValueStr[0]), atof(&lastTempValueStr[0])};
// }

// int selectDutyCycle(SensorData sensorData) {
//     float humidity = sensorData.humidity;
//     float temperature = sensorData.temperature;

//     if (humidity > 30) {
//         return 0;
//     }
//     else if (humidity <= 30 && humidity > 20) {
//         if (temperature < 25)
//         {
//             return 0;
//         }
//         else {
//             return 10;
//         }
//     }
//     else if (humidity <= 20 && humidity > 10) {
//         return 20;
//     }
//     else if (humidity <= 10) {
//         return 25;
//     }
//     else {
//         return 0;
//     }
// }

// void sendDecision(int dutyCycle) {
//     char decision[4];
//     itoa(dutyCycle, decision, 10);
//     decision[3] = BLUETOOTH_END_CHARACTER;
//     Serial.write(decision, 4);
// }

// void handleMessage(String message) {
//     SensorData sensorData = getSensorData(message);
//     int dutyCycle = selectDutyCycle(sensorData);
//     analogWrite(PWM_PIN, dutyCycle * (MAX_PWM_SPEED / 100));
//     sendDecision(dutyCycle);
// }

// void printOnLCD(SensorData sensorData, int dutyCycle) {
//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print("Temp: ");
//     lcd.print(sensorData.temperature);
//     lcd.print(" C");
//     lcd.setCursor(0, 1);
//     lcd.print("Humidity: ");
//     lcd.print(sensorData.humidity);
//     lcd.print(" %");
//     lcd.setCursor(0, 2);
//     lcd.print("Duty Cycle: ");
//     lcd.print(dutyCycle);
//     lcd.print("%");
// }

// void loop() {
//     String msgBuffer = "";
//     String msg = Serial.readString();
//     if (msg != "") {
//         msgBuffer += msg;
//         if (msgBuffer.indexOf(BLUETOOTH_END_CHARACTER) != -1) 
//         {
//             handleMessage(msgBuffer);
//             delay(10);
//             printOnLCD(getSensorData(msgBuffer), selectDutyCycle(getSensorData(msgBuffer)));
//             msgBuffer = "";
//         }
//     }
// }
// In the improved version of the code, the analogWrite() function is used to output the PWM signal to the PWM_PIN pin. The printOnLCD() function now displays the temperature, humidity, and duty cycle values on the LCD. The selectDutyCycle() function now has a default return statement to handle input values that do not match any of the conditions.

// Also, the sendDecision() function now uses a character array instead of a String object to send the duty cycle value over the serial port. The itoa() function is used to convert the integer duty cycle value to a character array, and the Serial.write() function is used to send the character array over the serial port. Finally, a delay of 10 milliseconds is added to the loop() function to prevent the code from executing too fast.