//------------------------------------------------------------------------------
// BASE FUNCTIONS
//------------------------------------------------------------------------------
// struct to hold sensor values and sensor fails info
struct SensorData {
  float temperature;      // celcius
  float humidity;         //
  float pressure;         // mb
  float lightIntensity;   // lux
  int raindrop;           // rain or not
  int uvRadiation;        //
  int windDirection;      // relative degree or can be 8 wind directions
  float windSpeed;        // meter / minute
  int co2;
  std::string error;      // configured as std::string, to simply access the existed string memory address, to avoid re-copying the string value
};

// naive error checking
bool hasError(void* value) {
  // perhaps at some point, a "0.00" or zero value reading is not a real error.
  // this function may give us a bug issue in the near future, so it should be monitored with caution.
  // more sophisticated error check mechanisms are urgently needed.
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
      case "humidity"_hash:
        data->humidity = *(float*)value;
        break;
      case "pressure"_hash:
        data->pressure = *(float*)value;
        break;
      case "lightIntensity"_hash:
        data->lightIntensity = *(float*)value;
        break;
      case "raindrop"_hash:
        data->raindrop = *(int*)value;
        break;
      case "uvRadiation"_hash:
        data->uvRadiation = *(int*)value;
        break;
      case "windDirection"_hash:
        data->windDirection = *(int*)value;
        break;
      case "windSpeed"_hash:
        data->windSpeed = *(float*)value;
        break;
      case "co2"_hash:
        data->co2 = *(int*)value;
        break;
      default:
        data->error += sensorName + ",";
        break;
    }
  } else {
    data->error += sensorName + ",";
  }
}

// send data to web server
void sendData(SensorData* data) {
  String url = "http://customwebserver.com/post?" +
                "temp=" + String(data->temperature) +
                "&hum=" + String(data->humidity) + 
                "&pressure=" + String(data->pressure) +
                "&lux=" + String(data->lightIntensity) + 
                "&rain=" + String(data->raindrop) +                 
                "&uv=" + String(data->uvRadiation) + 
                "&wind_direction=" + String(data->windDirection) + 
                "&wind_speed=" + String(data->windSpeed) +
                "&co2=" + String(data->co2) +
                "&error=" + String(data->error);           
  sendHttpRequest(url);
}

// SIM800L module http interfaces
void sendHttpRequest(String url) {
  sim800l.println("AT+HTTPINIT");
  delay(1000);
  sim800l.println("AT+HTTPPARA=\"URL\",\"" + url + "\"");
  delay(1000);
  sim800l.println("AT+HTTPACTION=0");
  delay(10000);

//------------------------------------------------------------------------------
// SENSOR READING FUNCTIONS
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

// get light lux
// This function calculates and returns the current light level in lux using the TSL2591 sensor.
// It measures the full luminosity, IR and full values and uses the manufacturer's formula to calculate the lux value.
// Returns the lux value as a floating-point number.
float getLux() {
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  float ratio = (float)ir / (float)full;
  float lux = 0.0;
  if (ratio < 0.50) {
    lux = 0.0304 * full - 0.062 * full * pow(ratio, 1.4);
  } else if (ratio < 0.61) {
    lux = 0.0224 * full - 0.031 * ir;
  } else if (ratio < 0.80) {
    lux = 0.0128 * full - 0.0153 * ir;
  } else if (ratio < 1.30) {
    lux = 0.00146 * full - 0.00112 * ir;
  }
  return lux;
}

// get rain status
// This function reads the input from a rain sensor connected to pin A0.
// It compares the sensor value to a threshold value and determines whether it's raining or not.
// Returns an integer value: 0 for dry, 1 for rain.
int getRainStatus() {
  int rainPin = A0;                         // set the input pin for the rain sensor
  // int thresholdValue = 500;              // set the threshold value for detecting rain
  int rainState = digitalRead(rainPin);     // read the current value from the rain sensor
  
  return (rainState == HIGH) ? 1 : 0;
}

// get uv index
int getUvIndex() {
  float uvIndex = uv.readUV();              // read the UV index value from the sensor
  uvIndex /= 100.0;                         // convert the value to an integer UV index by dividing by 100
  return (int) uvIndex;                     // cast the float value to an integer and return it
}

// get wind direction
int getWindDirection(int degAngle, int startAngle) {        // https://github.com/Nerdiyde/Anemosens/blob/main/software/anemosens_mcu/dir_sensing.ino
  // direction sensor variables
  // AMS_AS5048B dir_sensor;
  // float wind_direction = 0; // this holds the latest measured wind direction    
  SPI.beginTransaction(SPISettings(3000000, MSBFIRST, SPI_MODE1));
  digitalWrite(windDir_pin, LOW);
  SPI.transfer16(COMMAND_READ);
  digitalWrite(windDir_pin, HIGH);
  
  delay(10);
  
  digitalWrite(windDir_pin, LOW);
  uint16_t rawData = SPI.transfer16(COMMAND_READ);
  digitalWrite(windDir_pin, HIGH);
  SPI.endTransaction();
  
  rawData = rawData & 0b0011111111111111;
  float degAngle = (float)rawData / 16384.0 * 360.0;

  int finaldegree = int((degAngle - startAngle + 22.5) / 45) ;
  startAngle = degAngle;
  return finaldegree;
}