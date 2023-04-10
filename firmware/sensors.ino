//------------------------------------------------------------------------------
// SENSORS
//------------------------------------------------------------------------------

//Libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>              // temperature, humidity.     - DHT22   -40~80 celsius/+-0.5 celsius degree accuracy, 20 to 80% with 5% RH accuracy
#include <SFE_BMP180.h>       // Air pressure, temperature. - BMP180  (300-1100 hPa (9000m-500m above sea level), and -40°C-85°C with ±1.0°C accuracy.)
#include <Adafruit_TSL2591.h> // Light intensity            - TSL2591 Light Flux Sensor
#include <Adafruit_SI1145.h>  // UV radiation               - SI1145  UV sensor (seems like we can implement light flux sensing algirithm with this sensor too)
#include <SoftwareSerial.h>   // for sim Telkomsel          - SIM800L module

#include <SPI.h>
#include <ams_as5048b.h>      // Wind direction             - AS5048  the reading algorithm and the anemometer stl files are borrowed from Fabian Steppat's project at https://github.com/Nerdiyde/Anemosens
#include <CO2Sensor.h>        // CO2                        - MG811   CO2 sensor

void setupSensors() {
  if (!SPI.begin()) {
    Serial.println("Failed to initialize SPI");
    while (1); 
  }
  
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Failed to initialize BMP180 sensor");
    while (1);
  }

  if (!Wire.begin()) {
    Serial.println("Failed to initialize I2C");
    while (1);
  }
  
  if (!tsl.begin()) {
    Serial.println("Failed to initialize TSL sensor");
    while (1);
  }
  
  if (!uv.begin()) {
    Serial.println("Failed to initialize UV sensor");
    while (1);
  }
  
  pinMode(RAIN_PIN, INPUT);
  co2Sensor.calibrate();
  
  if (!sim800l.begin(9600)) {
    Serial.println("Failed to initialize SIM800L");
    while (1);
  }
  
  if (!bmp.begin(BMP_I2C_ADDRESS)) {
    Serial.println("Failed to initialize BMP sensor at I2C address 0x76");
    while (1);
  }

  init_wind_direction_sensor();
  init_wind_speed_sensor();
}

// read sensors
void readSensors() {                                 // read data from sensors and update the SensorData struct values.

  SensorData data;                                   // struct for sensor values
  
  // temperature
  // implementing calculation based on two kind of sensors with different accuracy
  // to demonstrate basic reliability and redundancy approach. dht22 and bmp180 sensor. 
  int status = bmp.startTemperature();              // BMP180 = return 1 if started successfully, 0 if there was an error.
  delay(status);
  if (dht.read22(DHT_PIN) && status) {
    float temp = getWeightedTemp(dht.Temperature(), bmp.getTemperature());
    assignSensorData(&temp, "temperature", &data);
}
  
  // humidity
  float hum = dht.humidity();
  assignSensorData(&hum, "humidity", &data);  
  
  // pressure in hPa (hectopascals).
  float pres = bmp.getPressure();
  assignSensorData(&pres, "pressure", &data);
  
  // lightIntensity in lux
  float lux = getLux()
  assignSensorData(&lux, "lightIntensity", &data);
  
  // raindrop
  // representing the raindrop amount. 
  int raindrop = getRainStatus()  
  assignSensorData(&raindrop, "raindrop", &data);
  
  // uvRadiation: the UV radiation level.
  int uv = getUvIndex()
  assignSensorData(&uv, "uvRadiation", &data)
  
  // windDirection: the wind direction.
  String directions[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
  int degreeDirection = getWindDirections();
  string windDirection = directions[degreeDirection % 8];  
  assignSensorData(&degreeDirection, "windDirection", &data)

  // windSpeed: a floating-point value representing the wind speed in meters per second.
  wspeed = digitalRead(2);
  assignSensorData(&wspeed, "windSpeed", &data)

  // co2: an integer value representing the CO2 level.
  int co2val = co2Sensor.read();  
  assignSensorData(&co2val, "co2", &data)

  // finally, send the result to webserver
  sendData(data)
}