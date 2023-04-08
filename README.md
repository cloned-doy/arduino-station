# Arduino Station Firmware : Experimental #

This is the firmware for an experimental Arduino project, developed using the ATmega328P microcontroller. It includes code for reading various sensors and putting the system into deep sleep to conserve power.

This arduino system are designed to be implemented in some solar panel based power system.

### Getting Started
To use this firmware, you will need an Arduino board and the necessary sensors. Simply upload the main.ino file to your Arduino board and connect the sensors according to the pin assignments in sensors.ino.

### Requirements
Arduino board, eg. Arduino Pro Mini
DHT22 temperature and humidity sensor
BMP180 pressure sensor
TSL2591 light sensor
SI1145 UV sensor
AMS AS5048B magnetic encoder
CO2 sensor
SIM800L module

### Usage
The firmware reads sensor data and saves it to a SensorData struct, which is then sent to a web server using the SIM800L module's HTTP interfaces. To change the server URL, modify the url variable in the sendData function.

To adjust the sleep interval, modify the WAKE_INTERVAL constant in main.ino.

### Contributing
This is an experimental library only. However, if you find any bugs or have ideas for new features, please create an issue on GitHub or submit a pull request.

### License
This project is licensed under the MIT License. See the LICENSE file for details.

### Acknowledgments
Thanks to Zettabyte for the opportunity to take this technical test.