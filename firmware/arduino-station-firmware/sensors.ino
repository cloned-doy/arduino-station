//------------------------------------------------------------------------------
// SENSORS
//------------------------------------------------------------------------------

//Libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>              // temperature, humidity.     - DHT22   -40~80 celsius/+-0.5 celsius degree accuracy, 20 to 80% with 5% RH accuracy
#include <Adafruit_BMP085.h>  // Air pressure, temperature. - BMP180  (300-1100 hPa (9000m-500m above sea level), and -40°C-85°C with ±1.0°C accuracy.)
#include <Adafruit_TSL2591.h> // Light intensity            - TSL2591 Light Flux Sensor
#include <Adafruit_SI1145.h>  // UV radiation               - SI1145  UV sensor (seems like we can implement light flux sensing algirithm with this sensor too)
#include <SoftwareSerial.h>

#include <SPI.h>
#include <ams_as5048b.h>      // Wind direction             - AS5048  the reading algorithm and the anemometer stl files are borrowed from Fabian Steppat's project at https://github.com/Nerdiyde/Anemosens
#include <CO2Sensor.h>        // CO2                        - MG811   CO2 sensor
#include "functions.h"
//                            // Wind speed                 - A3144 Hall Effect Sensor
//                            // Raindrop                   - YL-83
//                            // sim Telkomsel              - SIM800L module

//Constants
//DHT22 sensor 
#define DHTPIN 2            // DHT22 pin
#define DHTTYPE DHT22       // (AM2302)
DHT dht(DHTPIN, DHTTYPE);   

// YL-83 rain sensor pin 
const int rainPin = 2; 

// wind directions
const byte windDir_pin = 10; 
uint16_t rawData = 0;
float degAngle = 0;
float startAngle = 0;
float correctedAngle = 0;
const uint16_t COMMAND_READ = 0xFFF; // read command

//Variables
Adafruit_BMP085 bmp;                                // BMP180
Adafruit_TSL2591 tsl  = Adafruit_TSL2591(2591);
Adafruit_SI1145 uv;                                 // create an instance of the UV sensor object
O2Sensor co2Sensor(A0, 0.99, 100);
SoftwareSerial sim800l(7, 8);                       // RX, TX


// error handling in this setup func need to be implemented soon.
void setupSensors() {
  SPI.begin();  
  Serial.begin(9600);
  dht.begin();  
  bmp.begin()
  Wire.begin();
  tsl.begin();    
  uv.begin(); // initialize the UV sensor
  pinMode(rainPin, INPUT); // rain
  pinMode(windDir_pin, OUTPUT);  // wind direction
  co2Sensor.calibrate();  
  
  sim800l.begin(9600);
  bmp.begin(0x76); // Set the I2C address to 0x76 (default)

  init_wind_direction_sensor();
  init_wind_speed_sensor(); 
}

// read data from sensors and update the SensorData struct values.
void readSensors() {
  SensorData data; // struct for sensor values
  
  // temperature
  // implementing calculation based on two kind of sensors with different accuracy
  // to demonstrate basic reliability and redundancy approach. dht22 and bmp180 sensor.
  float temp = getWeightedTemp(dht.readTemperature(), bmp.readTemperature());
  assignSensorData(&temp, "temperature", &data);
  
  // humidity
  float hum = dht.readHumidity();
  assignSensorData(&hum, "humidity", &data);  
  
  // pressure
  // air pressure in hPa (hectopascals).
  float pres = bmp.readPressure();
  assignSensorData(&pres, "pressure", &data);
  
  // lightIntensity
  // light intensity in lux
  float lux = getLux()
  assignSensorData(&lux, "lightIntensity", &data);
  
  // raindrop
  // representing the raindrop amount. 
  int raindrop = getRainStatus()  
  assignSensorData(&raindrop, "raindrop", &data);
  
  // uvRadiation: an integer value representing the UV radiation level.
  int uv = getUvIndex()
  assignSensorData(&uv, "uvRadiation", &data)
  
  // windDirection: an integer value representing the wind direction.
  String directions[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
  int degreeDirection = getWindDirections();
  string windDirection = directions[degreeDirection % 8];  
  assignSensorData(&degreeDirection, "windDirection", &data)

  // windSpeed: a floating-point value representing the wind speed in meters per second.
  // data->hallEffect = digitalRead(2);

  // co2: an integer value representing the CO2 level.
  int co2val = co2Sensor.read();  
  assignSensorData(&co2val, "co2", &data)

  // finally, send the result to webserver
  sendData(data)
}