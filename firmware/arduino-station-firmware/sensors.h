#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <dht11.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_TSL2591.h>
#include <SoftwareSerial.h>

#define DHTPIN1 2
#define DHTPIN2 3

void setupSensors();
void readSensors();
void handleErrors();

#endif
