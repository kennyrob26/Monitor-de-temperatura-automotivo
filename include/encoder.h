#ifndef __ENCODER__
    #define __ENCODER__

    #include<Arduino.h>
    #include "STM32encoder.h"
    #include "display.h"
    #include "eeprom.h"

    extern STM32encoder encoder;

    void measureButtonPressTime();
    void configEncoder();
    void checkEncoderInteraction();

    void encoderSetsTemperatureEngine();
    void encoderSetsDisplayBrightness();

    bool encoderMovesRight();


#endif