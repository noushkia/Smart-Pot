#include <Arduino.h>
#include <Wire.h>

#define SENSOR_ADDR 0x40
#define BLUETOOTH_BAUD_RATE 9600
#define UNDEFINED -100
#define DELIMITER '#'

float lastHumidity = UNDEFINED;

float getHumidity() {
  unsigned int data[2];
  Wire.beginTransmission(SENSOR_ADDR);  

  // Send humidity measurement command
  Wire.write(0xF5);  
  Wire.endTransmission();

  // Request 2 bytes of data  
  Wire.requestFrom(SENSOR_ADDR, 2);
  while (Wire.available() < 2);
  data[0] = Wire.read();    
  data[1] = Wire.read();
  return (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
}

float getTemperature() {
  unsigned int data[2];
  Wire.beginTransmission(SENSOR_ADDR);  

  // Send temperget_temp_celciusature measurement command 
  Wire.write(0xF3);  
  Wire.endTransmission();  

  // Request 2 bytes of data  
  Wire.requestFrom(SENSOR_ADDR, 2);

  while (Wire.available() < 2);     
  data[0] = Wire.read();    
  data[1] = Wire.read();
  return (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
}

void sendSensorData(float humidity, float temperature) {
  Serial.print(humidity);
  Serial.print(DELIMITER);
  Serial.print(temperature);
  Serial.print(DELIMITER);
}

void setup() {
  Wire.begin();
  Serial.begin(BLUETOOTH_BAUD_RATE);
}

void loop() {
  // float currHumidity = getHumidity();
  // float currTemp = getTemperature();
  // if (fabs(currHumidity - lastHumidity) > 5.0 || lastHumidity == UNDEFINED) {
  //     lastHumidity = currHumidity;
  //     sendSensorData(currHumidity, currTemp);
  // }
  Serial.print("KIR");
  delay(10000);
}