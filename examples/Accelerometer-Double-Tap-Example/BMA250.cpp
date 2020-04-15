#include "BMA250.h"
#include <Arduino.h>
#include <Wire.h>


BMA250::BMA250()
{
}

void BMA250::begin(uint8_t range, uint8_t bw)
{
  delay(1);
  //soft reset
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(0x14);
  Wire.write(0xB6);
  Wire.endTransmission();
  delay(5);
  //Setup the range measurement setting
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(BMA250_G_RANGE);
  Wire.write(range);
  Wire.endTransmission();
  delay(1);
  //Setup the bandwidth
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(BMA250_BANDWIDTH);
  Wire.write(bw);
  Wire.endTransmission();
  delay(1);
}

void BMA250::doubleTapInit(){
  //Setup the double tap interrupt
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(0x2A);
  Wire.write((0 << 6)  | (1 << 2) | (1 << 0));
  Wire.endTransmission();
  delay(1);
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(BMA250_TAP_TH);
  Wire.write(BMA250_tap_threshold);
  Wire.endTransmission();
  delay(1);
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(BMA250_INT_EN);
  Wire.write(BMA250_d_tap_en);
  Wire.endTransmission();
  delay(1);
  clearInterrupt();
}

int BMA250::clearInterrupt()
{
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(BMA250_INT_MODE);
  Wire.write(BMA250_int_latch_and_reset);
  Wire.endTransmission();
  delay(1);
  doubleTap = 0;
  return 0;
}

int BMA250::read()
{
  //Set register index
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(0x02);
  Wire.endTransmission();
  delay(1);
  //Request data bytes
  Wire.requestFrom(BMA250_I2CADDR, 8);
  delay(1);
  //Receive acceleration measurements as 16 bit integers
  X = (int16_t)Wire.read();
  X |= (int16_t)Wire.read() << 8;
  Y = (int16_t)Wire.read();
  Y |= (int16_t)Wire.read() << 8;
  Z = (int16_t)Wire.read();
  Z |= (int16_t)Wire.read() << 8;
  //The following shifts preserve the sign bits
  X >>= 6; Y >>= 6; Z >>= 6;
  //Receive temperature measurement
  rawTemp = Wire.read();
  uint8_t tapInt = Wire.read();

  if (tapInt & BMA250_d_tap_int)
    doubleTap = 1;
  
  return 0;
}

int BMA250::suspend()
{//0x11 - deep suspend 1 while suspend is 0?
  //0x80 - suspend
  //0x20 - deep suspend
  Wire.beginTransmission(BMA250_I2CADDR);
  Wire.write(0x11);
  Wire.write(0x20);
  Wire.endTransmission();
  delay(1);
  doubleTap = 0;
  return 0;
}
