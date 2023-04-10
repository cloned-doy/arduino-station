//------------------------------------------------------------------------------
// Arduino Station's firmware
// built with love, powered by ATmega328P microcontroller.

// by adityabachtiar996@gmail.com -- // http://www.github.com/cloned-doy/arduino-station
// thanks to Zettabyte for the chance to take this beautiful technical test.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// INCLUDES
//------------------------------------------------------------------------------
#include <LowPower.h>               // library for power-saving mode
#include <Watchdog.h>

#include "config.h"
#include "defs.h"

void setup() {
  setupSensors();                   // initialize all sensors
}

void loop() {
  readSensors();                    // read sensor data
  Watchdog.reset();                 // reset watchdog timer to prevent system crashes
  LowPower.powerDown(SLEEP_15MIN, ADC_OFF, BOD_OFF); // put system into deep sleep for 15 minutes
}