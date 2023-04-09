
// wind directions
uint16_t rawData = 0;
float degAngle = 0;
float startAngle = 0;
float correctedAngle = 0;
const uint16_t COMMAND_READ = 0xFFF; // read command

//Variables
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;                                      // BMP180
Adafruit_TSL2591 tsl(TSL_I2C_ADDRESS);
Adafruit_SI1145 uv = Adafruit_SI1145(UV_SI1145_ADDRESS);  // create an instance of the UV sensor object
O2Sensor co2Sensor(A0, 0.99, 100);
SoftwareSerial sim800l(7, 8);                             // sim800l RX, TX

// Define the software serial pins for the SIM800L module
SoftwareSerial sim800lSerial(7, 8);

