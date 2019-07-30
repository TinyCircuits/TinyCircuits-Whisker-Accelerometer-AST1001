/*************************************************************************
 * BMA250 WhiskerBoard Tutorial:
 * This example program will show the basic method of printing out the 
 * accelerometer values from the BMA250 to the TinyScreen+, Serial Monitor, 
 * and the Serial Plotter
 * 
 * Hardware by: TinyCircuits
 * Code by: Laverena Wienclaw for TinyCircuits
 *
 * Initiated: Mon. 11/29/2017 
 * Updated: Tue. 07/03/2018
 ************************************************************************/
 
#include <Wire.h>         // For I2C communication with sensor
#include <TinyScreen.h>   // For interfacing with the TinyScreen+
#include "BMA250.h"       // For interfacing with the accel. sensor

// TinyScreen+ variables
TinyScreen display = TinyScreen(TinyScreenPlus);
int background = TS_8b_Black;

// Accelerometer sensor variables for the sensor and its values
BMA250 acccel_sensor;
int x, y, z, temp;

// The power pin for our board, used for digitally writing to output
const int powerPin = 4;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Setting variables to ouput digitally
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);

  // Setup and style for TinyScreen+
  display.begin();
  display.setFlip(true);
  display.setBrightness(15);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, background);

  // Just displaying a quick message to show program has been uploaded
  // The following three lines can be removed
  display.setCursor(0, 0);
  display.print("Initializing BMA...");
  delay(1000);

  // Set the cursor to the following coordinates before it prints "BMA250 Test" 
  selectPort(0);
  acccel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms); // Sets up the BMA250 acccel_sensorerometer
  display.clearScreen();
}

void loop() {
  // This will make the screen look a little unsteady but is needed in order
  // to clear old values 
  display.clearScreen();
  
  acccel_sensor.read();//This function gets new data from the acccelerometer

  // Get the acceleration values from the sensor and store them into local variables
  // (Makes reading the rest of the program easier)
  x = acccel_sensor.X;
  y = acccel_sensor.Y;
  z = acccel_sensor.Z;
  temp = ((acccel_sensor.rawTemp * 0.5) + 24.0, 1);

 // If the BME280 is not found, nor connected correctly, these values will be produced
 // by the sensor 
  if (x == -1 && y == -1 && z == -1) {
    // Print error message to Serial Monitor
    Serial.print("ERROR! NO BMA250 DETECTED!");

    // Display an error on the TinyScreen+
    display.setCursor(0, 48);
    display.fontColor(TS_8b_Red, background);
    display.print("ERROR! NO BMA250 Sensor");
    display.fontColor(TS_8b_White, background);
    delay(100);
  }
  else { // if we have correct sensor readings: 
    showSerial();                 //Print to Serial Monitor 
    showGraph(x, y, z);           //Show on Serial Plotter
    showDisplay(x, y, z, temp);   //Display on TinyScreen 
  }

  // The BMA250 can only poll new sensor values every 64ms, so this delay
  // will ensure that we can continue to read values
  delay(250);
  // ***Without the delay, there would not be any sensor output*** 
}

// **This function is necessary for all Whisker boards attached through an Adapter board**
// Selects the correct address of the port being used in the Adapter board
void selectPort(int port) {
  Wire.beginTransmission(0x70);
  Wire.write(0x04 + port);
  Wire.endTransmission();
}

// Prints the sensor values to the Serial Monitor (found under 'Tools')
void showSerial() {
  SerialUSB.print("X = ");
  SerialUSB.print(x);
  
  SerialUSB.print("  Y = ");
  SerialUSB.print(y);
  
  SerialUSB.print("  Z = ");
  SerialUSB.print(z);
  
  SerialUSB.print("  Temperature(C) = ");
  SerialUSB.println((acccel_sensor.rawTemp * 0.5) + 24.0, 1);
}

//Displays sensor data on the TinyScreen+ 
void showDisplay(int a, int b, int c, int d) {
  display.setCursor(0, 0);
  display.print("BMA250 Readings:");
  
  display.setCursor(0, 12);
  display.print("X = ");
  display.print(a);

  display.setCursor(0, 24);
  display.print("Y = ");
  display.print(b);

  display.setCursor(0, 36);
  display.print("Z = ");
  display.print(c);
  
  display.setCursor(0, 48);
  display.print("T = ");
  display.print(d);
}

//Plots X, Y and Z data on Serial Plotter (found under 'Tools')
void showGraph(int x, int y, int z) {
  SerialUSB.print(x);
  Serial.print(",");

  Serial.print(y);
  Serial.print(",");

  Serial.print(z);
  Serial.print(",");
}
