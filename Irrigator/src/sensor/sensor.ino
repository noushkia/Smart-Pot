#include <Wire.h>
#include <Arduino.h>

#define SENSOR_ADDR 0x40
#define BLUETOOTH_DELIMETER_CHARACTER '-'
#define BLUETOOTH_END_CHARACTER '/'
#define BLUETOOTH_BAUD_RATE 9600

float lastHumidity = 0.0;

// read sensor docs
float readHumidity() {
  unsigned int data[2];
  Wire.beginTransmission(SENSOR_ADDR);  

  // Send humidity measurement command, NO HOLD master  
  Wire.write(0xF5);  
  Wire.endTransmission();
  
  // Request 2 bytes of data  
  Wire.requestFrom(SENSOR_ADDR, 2);
  if(Wire.available() == 2){
    data[0] = Wire.read();    
    data[1] = Wire.read();
    return (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
  }
}

// read sensor docs
float readTemperature(){
  unsigned int data[2];
  Wire.beginTransmission(SENSOR_ADDR);  

  // Send temperget_temp_celciusature measurement command, NO HOLD master  
  Wire.write(0xF3);  
  Wire.endTransmission();  
  
  // Request 2 bytes of data  
  Wire.requestFrom(SENSOR_ADDR, 2);
  
  if(Wire.available() == 2)  
  {    
    data[0] = Wire.read();    
    data[1] = Wire.read();
    return (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
  }
}

void setup()
{  
  Wire.begin();
  Serial.begin(BLUETOOTH_BAUD_RATE);
  Serial.setTimeout(0);
}

void sendSensorData(float humidity, float temperature){
  String message = String(humidity) + BLUETOOTH_DELIMETER_CHARACTER + String(temperature) + BLUETOOTH_END_CHARACTER;
  Serial.write(&message[0]);
}

void loop() {
    float cuurHumidity = readHumidity();
    float currTemp = readTemperature();
    if (abs(cuurHumidity - lastHumidity) > 5.0 || lastHumidity == 0.0) {
        lastHumidity = cuurHumidity;
        sendSensorData(cuurHumidity, currTemp);
    }
}
