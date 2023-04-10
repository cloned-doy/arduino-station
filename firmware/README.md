## Arduino Station Firmware

- The firmware is designed to run on an ATmega328P microcontroller and utilizes various sensors to collect environmental data, such as temperature, humidity, pressure, light intensity, raindrop, UV radiation, wind direction and speed, and CO2.

- The firmware utilizes the LowPower library to put the system into deep sleep for 15 minutes to conserve power between sensor readings.

- The sensor data is stored in a SensorData struct, which also tracks any sensor reading failures.

- We utilizes the SIM800L module to send HTTP POST requests to a custom web server with the collected sensor data and error messages.

### further development 
- fix (or replace) the naive error checking mechanisms to prevent false sensor error readings. 
   
 - remote firmware monitoring (error check, or even command the board to reboot accordingly)

 - over-the-air (OTA) updates to improve maintainability and ease of use (seems like arduino pro mini doesnt supported this functionality yet)
  

### plese take care the example.ino file before uploading the firmware to the board.