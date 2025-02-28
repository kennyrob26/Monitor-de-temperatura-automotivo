/*************************************************** 
  This is a library for the Adafruit PT100/P1000 RTD Sensor w/MAX31865

  Designed specifically to work with the Adafruit RTD Sensor
  ----> https://www.adafruit.com/products/3328

  This sensor uses SPI to communicate, 4 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef __SENSORMAX31865__
    #define __SENSORMAX31865__

    #include <Arduino.h>
    #include "Adafruit_MAX31865.h"
    #include <Wire.h>
    #include "eeprom.h"


    void configMax31865();
    uint8_t engineTemperature();
    void fautsMAX31865();

    uint8_t getMaxTemperatureEngine();
    void setEngineOverheatThreshold(u_int8_t maxTemperature);

    void updateMaxRecordedTemperature(uint8_t temperatureNow);
    uint8_t getMaxRecordedTemperature();

    void updateMinRecordedTemperature(uint8_t temperatureNow);
    uint8_t getMinRecordedTemperature();
    void resetRecordedTemperatures();





#endif