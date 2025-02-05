#ifndef __DISPLAY__
    #define __DISPLAY

    #include <Arduino.h>
    #include "TM1637Display.h"
    #include "sensorMax31865.h"
    #include "STM32encoder.h"

    extern STM32encoder encoder;
    extern bool firstOccurrence;
    extern int8_t brightness;

    void configDisplay();
    void setBrightnessDisplay();
    void showTemperature();
    void setMaxTemperature();
    void turnOffDisplay();
    uint8_t getMaxSetTemperature();

#endif