#ifndef FUNC_H
#define FUNC_H

#include <string>
#include "Adafruit_TSL2591.h"

// struct to hold sensor values and sensor fails info
struct SensorData {
    float temperature;
    float humidity;
    float pressure;
    float lightIntensity;
    int raindrop;
    int uvRadiation;
    int windDirection;
    float windSpeed;
    int co2;
    std::string error;
};

// naive error checking
bool hasError(void* value);

// save sensor reading values into SensorData struct
void assignSensorData(void* value, const std::string& sensorName, SensorData* data);

// send data to web server
void sendData(SensorData* data);

// SIM800L module http interfaces
void sendHttpRequest(String url);

// get temperature
float getWeightedTemp(float dht22Temperature, float bmp180Temperature);

// get light lux
float getLux();

#endif // FUNC_H

#ifndef SENSOR_READINGS_H
#define SENSOR_READINGS_H

const int windDir_pin = 10;
const uint16_t COMMAND_READ = 0b0100000000000000;

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

// get temperature
float getWeightedTemp(float dht22Temperature, float bmp180Temperature);

// get light lux
float getLux();

int getRainStatus();

int getUvIndex();

int getWindDirection(int degAngle, int startAngle);

#endif // SENSOR_READINGS_H
