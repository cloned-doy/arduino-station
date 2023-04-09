//------------------------------------------------------------------------------
// CONFIG
//------------------------------------------------------------------------------

// Pin placement for sensors on Arduino Pro Mini
#define DHT_PIN 8               // DHT22 temperature and humidity sensor
#define DHTTYPE DHT22       
#define SIM_TX_PIN 4            // SIM800L module TX pin
#define SIM_RX_PIN 3            // SIM800L module RX pin
#define WD_CS_PIN 10            // AS5048B wind direction sensor chip select pin
#define WD_DO_PIN 11            // AS5048B wind direction sensor data output pin
#define CO2_DIGITAL_PIN 6       // MG811 CO2 sensor digital output pin
#define RAIN_PIN 7              // rain pin

// Default I2C addresses for sensors
#define SDA_PIN A4       
#define SCL_PIN A5       
#define BMP_I2C_ADDRESS 0x77    // BMP180 air pressure and temperature sensor
#define TSL_I2C_ADDRESS 0x29    // TSL2591 light intensity sensor
#define UV_SI1145_ADDRESS 0x60  // SI1145 UV radiation sensor

// SIM800L and SIM card config
#define APN "internet"          // Telkomsel APN

// sleep timer
#define SLEEP_5_MINUTES 300000  // 5 minutes in ms
#define SLEEP_15_MINUTES 900000 // 15 minutes in ms
#define SLEEP_30_MINUTES 1800000 // 30 minutes in ms
#define SLEEP_1_5_HOUR 5400000  // 1.5 hours in ms

// notes
// wind speed and wind direction stl implementing the Nerdy.de's design at https://github.com/Nerdiyde/Anemosens
