#ifndef __DISPLAY__
    #define __DISPLAY

    #include <Arduino.h>
    #include "TM1637Display.h"
    #include "sensorMax31865.h"

    extern int8_t displayBrightness;

    void configDisplay();
    void updateDisplayValue(int8_t value, uint8_t length);
    void showTemperature();  
    void turnOffDisplay();

    void setDisplayBrightness(int8_t brightness); 
    int8_t getDisplayBrightness();

#endif