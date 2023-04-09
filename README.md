

# Arduino Station Firmware : Experimental #

This is the firmware for an experimental Arduino project, developed using the ATmega328P microcontroller. It includes code for reading various sensors and putting the system into deep sleep to conserve power.

This arduino system are designed to be implemented in solar panel based power system. 

For reference, here is the [project report](https://docs.google.com/document/d/11zYSdMaM7gJ9rtgTeNqNHVAx77eldqqeS4g4kZ2feqw/edit?usp=sharing)  and the [bill of materials](https://docs.google.com/spreadsheets/d/1Wg0oTnzC-42zgrJtUEOQMCml9h45T37ViTcRstmF_0g/edit?usp=sharing).

### Getting Started
To use this firmware, you will need an Arduino board and the necessary sensors. Simply upload the firmware.ino file to your Arduino board and connect the sensors according to the pin assignments in the PCB.

### Functions
-   Temperature
-   Humidity
-   Pressure 
-   Light intensity 
-   Raindrop
-   UV radiation 
-   Wind direction 
-   Wind speed 
-   CO2 
-   Error monitoring

### Requirements
- Arduino Pro Mini
- DHT22 temperature and humidity sensor
- BMP180 pressure and temperature sensor
- rain sensor
- TSL2591 light sensor
- SI1145 UV sensor
- AMS AS5048B magnetic encoder - wind direction
- A3144 hall effect sensor - wind speed
- CO2 sensor
- SIM800L module

### Usage
The firmware reads sensor data and saves it to a SensorData struct, which is then sent to a web server using the SIM800L module's HTTP interfaces. To change the server URL, modify the url variable in the sendData function.

### Contributing
This is an experimental library only. However, if you find any bugs or have ideas for new features, please create an issue on GitHub or submit a pull request.

### License
This project is licensed under the MIT License. See the LICENSE file for details.

### Acknowledgments
Thanks to Zettabyte for the opportunity to take this technical test.