#ifndef __EEPROM_H__
    #define __EEPROM_H__

    #include <EEPROM.h>

    void configEEPROM();
    void writeMaxTemperatureEEPROM(uint8_t maxTemperature);
    uint8_t readMaxTemperatureEEPROM();
    void resetEEPROM();
#endif