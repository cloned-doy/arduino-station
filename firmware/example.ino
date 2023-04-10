
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// EXAMPLE FILE
// here is the code snippet to demonstrate 
// the temperature readings under the hood.
// actual code are refactored and separated into 3 files.
//------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// defs.h
// define libraries and setup variables
//------------------------------------------------------------------------------

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>              
#include <SFE_BMP180.h>       
#include "functions.h"

#define DHT_PIN 2
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);
SFE_BMP180 bmp;


////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// functions.ino
// specific file to declare some custom functions
//------------------------------------------------------------------------------

// naive error checking.
// better error check mechanisms are urgently needed.
bool hasError(void* value) {
  if (value == nullptr || *(int*)value == 0 || *(float*)value == 0.0 || ((String*)value)->isEmpty()) {
    return true;
  }
  return false;
}

// save sensor readingvalues into SensorData struct
void assignSensorData(void* value, const std::string& sensorName, SensorData* data) {
  if (!hasError(value)) {
    switch(hash(sensorName)) {
      case "temperature"_hash:
        data->temperature = *(float*)value;
        break;
      // the other hash check
    }
  } else {
    data->error += sensorName + ",";
  }
}


//------------------------------------------------------------------------------
// MULTI SENSOR CALCULATION FUNCTIONS
//------------------------------------------------------------------------------

// get temperature
// - dht22Temperature: the temperature reading from the DHT22 sensor
// - bmp180Temperature: the temperature reading from the BMP180 sensor
// Returns: the weighted average temperature
float getWeightedTemp(float dht22Temperature, float bmp180Temperature) {  
  // Weight = 1 / Variance, where variance = sum of (each data point - mean)^2 / number of data points.
  // Fixed weights has been calculated for each sensor, for faster computation.
  const float w1 = 0.33;                    // Weight for DHT22 temperature
  const float w2 = 0.67;                    // Weight for BMP180 temperature
  
  return (w1 * dht22Temperature + w2 * bmp180Temperature) / (w1 + w2);
}



////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// sensors.ino
// declare sensor reading functions
//------------------------------------------------------------------------------

void readSensors() {
  SensorData data;  // declare struct obj as 'data'
  int status = bmp.startTemperature();
  delay(status);
  if (dht.read22(DHT_PIN) && status) {
    float temp = getWeightedTemp(dht.temperature, bmp.getTemperatureC());
    assignSensorData(&temp, "temperature", &data);
    // read other sensors ...
  }
  
  // send the result via sim800l module to the web server
  // using http interfaces
  sendData(data);
}

////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// firmware.ino
// here is the main firmware
//------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Failed to initialize BMP180 sensor");
    while (1);
  }
}

void loop() {
  readSensors();            
  LowPower.powerDown(SLEEP_15MIN, ADC_OFF, BOD_OFF);
}
