#ifndef SENSOR_H
#define SENSOR_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_TSL2591.h>
#include <Adafruit_SI1145.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <ams_as5048b.h>
#include <CO2Sensor.h>
#include "functions.h"

#define DHTPIN 2
#define DHTTYPE DHT22
const int rainPin = 2;
const byte windDir_pin = 10;
const uint16_t COMMAND_READ = 0xFFF;

struct SensorData {
  float temperature;
  float humidity;
  float pressure;
  float lightIntensity;
  int raindrop;
  int uvRadiation;
  int windDirection;
  int co2;
};

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
Adafruit_SI1145 uv;
O2Sensor co2Sensor(A0, 0.99, 100);
SoftwareSerial sim800l(7, 8);

void setupSensors();
void readSensors();

#endif
