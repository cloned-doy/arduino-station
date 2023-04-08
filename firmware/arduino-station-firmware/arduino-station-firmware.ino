//------------------------------------------------------------------------------
// Arduino Station's firmware
// built with love and powered by ATmega328P microcontroller.

// by adityabachtiar996@gmail.com
// please see http://www.github.com/cloned-doy/arduino-station for more information. 

// thanks to Zettabyte for the chance to take this beautiful technical test.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// INCLUDES
//------------------------------------------------------------------------------
#include <LowPower.h>
#include "sensors.h"

void setup() {
  setupSensors();
}

void loop() {
  readSensors();
  Watchdog.reset();                       // reset watchdog timer to prevent system crashes
  LowPower.sleep(WAKE_INTERVAL * 1000);   // put system into deep sleep
}
