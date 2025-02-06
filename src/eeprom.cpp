#include "eeprom.h"
#define ADDRESS_MAX_TEMPERATURE 0

uint8_t maxTemperatureEEPROM = 0;

/// @brief Configuração inicial da EEPROM
void configEEPROM()
{
    EEPROM.begin();
}

/**
 * @brief Registra a temperatura critica na EEPROM
 * @param maxTemperature Valor da temperatura crítica a ser armazenado (0 a 150 ºC)
 */
void writeMaxTemperatureEEPROM(uint8_t maxTemperature)
{
    maxTemperatureEEPROM = readMaxTemperatureEEPROM();

    if(maxTemperature >= 0 && maxTemperature <= 150)
    {
        EEPROM.update(ADDRESS_MAX_TEMPERATURE, maxTemperature);
    }
    
}

/// @brief Realiza a leitura da temperatura limite registrada na EEPROM
/// @return Temperatura limite registrada na EEPROM (0 a 150ºC) (uint8_t)
uint8_t readMaxTemperatureEEPROM()
{
    maxTemperatureEEPROM = EEPROM.read(ADDRESS_MAX_TEMPERATURE);
    return maxTemperatureEEPROM;
}

/**
 *  @brief Reseta os valores da EEPROM para valores padrão, por padrão os valores são:
 * 
 *  `maxTemperatureEEPROM` -> 90ºC
 */
void resetEEPROM()
{
    writeMaxTemperatureEEPROM(90);
}