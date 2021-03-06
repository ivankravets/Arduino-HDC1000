
#include <Wire.h>
#include <Arduino.h>
#include "HDC1000.h"

HDC1000::HDC1000(int address)
{
  _address = address;
}

void HDC1000::begin()
{
  Wire.begin();
  configure();
}


void HDC1000::configure()
{
  delay(15);
  uint8_t commands[] = {
    HDC1000_CONFIGURATION_ADDRESS,
    0b00000000, // LBR MODE 0, 14-bit
    0b00000000
  };
  int i, length = sizeof(commands);

  Wire.beginTransmission(_address);
  for (i = 0; i < length; i++) {
    Wire.write(commands[i]);
  }
  Wire.endTransmission(_address);
}

float HDC1000::getTemperature()
{
  // uint16_t for <<8 bit-shift
  uint16_t d1;
  uint8_t d2;

  Wire.beginTransmission(_address);
  Wire.write(HDC1000_TEMP_ADDRESS);
  Wire.endTransmission(_address);

  delay(7);

  Wire.requestFrom(_address, (uint8_t)2);
  d1 = Wire.read();
  d2 = Wire.read();

  return ((d1<<8 | d2) / 65536.0 * 165) - 40;
}

float HDC1000::getHumidity()
{
  uint16_t d1;
  uint8_t d2;

  Wire.beginTransmission(_address);
  Wire.write(HDC1000_HUMI_ADDRESS);
  Wire.endTransmission(_address);

  delay(7);

  Wire.requestFrom(_address, (uint8_t)2);
  d1 = Wire.read();
  d2 = Wire.read();

  return (d1<<8 | d2) / 65536.0 * 100;
}

