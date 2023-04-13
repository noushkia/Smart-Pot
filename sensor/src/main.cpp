#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <SHT2x.h>

#define BLUETOOTH_TX_PIN 0 // Bluetooth module TX pin
#define BLUETOOTH_RX_PIN 1 // Bluetooth module RX pin

SoftwareSerial bluetooth(BLUETOOTH_TX_PIN, BLUETOOTH_RX_PIN); // Initialize Bluetooth serial
SHT2x sensor;
float last_humidity = 0.0;

void setup() {
  bluetooth.begin(9600);
  Wire.begin();
  if (!sensor.begin()) {
    bluetooth.println("Error: SHT25 sensor not found!");
    while (1);
  }
}

void loop() {
  float temperature = sensor.readTemperature();
  float humidity = sensor.readHumidity();

  String data = String(temperature) + "," + String(humidity);
  if (abs(humidity - last_humidity) > 5.0 || last_humidity == 0.0) {
    last_humidity = humidity;
    bluetooth.println(data);
  }
  delay(1000);
}