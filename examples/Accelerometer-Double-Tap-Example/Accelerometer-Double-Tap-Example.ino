//-----------------------------------------------------------------
// TinyCircuits BMA250 Double Tap Example
// This example is mutually exclusive from other distributions of the
// TinyCircuits BMA250 examples.
//
// Written by Ben Rose for TinyCircuits, http://TinyCircuits.com
// Last Updated: January 2020
//-----------------------------------------------------------------

#include <Wire.h>
#include "BMA250.h"
#include <Wireling.h>

// Universal Serial Monitor Config
#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

BMA250 accel;

void setup(void) {
  Wireling.begin();
  SerialMonitorInterface.begin(115200);
  delay(100);
  Wire.begin();
  accel.begin(BMA250_range_8g, BMA250_update_time_4ms);
  accel.doubleTapInit();
  accel.read();
  Wireling.selectPort(0);
}


void loop() {
  accel.read();
//  SerialMonitorInterface.print("x: ");
//  SerialMonitorInterface.print(accel.X);
//  SerialMonitorInterface.print(", y: ");
//  SerialMonitorInterface.print(accel.Y);
//  SerialMonitorInterface.print(", z:");
//  SerialMonitorInterface.print(accel.Z);
//  SerialMonitorInterface.print(",  t: ");
//  SerialMonitorInterface.print(accel.rawTemp / 2 + 24);
//  SerialMonitorInterface.println("degC");

  if (accel.doubleTap) {
    SerialMonitorInterface.print(millis());
    SerialMonitorInterface.println("Double tap detected!");
    accel.clearInterrupt();
  }
}
