#include "display.h"

#define CLK_PIN PB13
#define DIO_PIN PB12

uint64_t lastDisplayUpdate = 0;

TM1637Display display(CLK_PIN, DIO_PIN);

int8_t displayBrightness  = 7;
bool displayState = 1;

/// @brief Configurações iniciais do display
void configDisplay()
{
    display.setBrightness(displayBrightness);
    display.showNumberDec(0, false);
}

/**
 * @brief Atualiza o valor exibido pelo display
 * @param value valor que será exibido pelo display (0 a 255)
 * @param length quantidade de dígitos
 */
void updateDisplayValue(int8_t value, uint8_t length)
{
    display.showNumberDec(value, false, length, 1);
}

/// @brief Exibe a temperatura atual no display
void showTemperature()
{   
    uint8_t temperature = engineTemperature();

    if(isEngineTemperatureCritical())
    {
        if((millis() - lastDisplayUpdate ) > 250)
        {
            if(displayState == true)
            {
                setDisplayBrightness(7);
                displayState = false;
            }
            else
            {
                setDisplayBrightness(-1);
                displayState = true;
            }
            lastDisplayUpdate = millis();
        }
    }

    updateDisplayValue(temperature, 3);
}

/**
 *  @brief Define o brilho do display
 *  @param brightness intensidade do brilho (-1,7), 
 *  |  -1 -> desligado  |  7 -> brilho maximo
 */
void setDisplayBrightness(int8_t brightness)
{
    displayBrightness = brightness;
    if(displayBrightness == -1)
        display.setBrightness(7, false);
    else
        display.setBrightness(displayBrightness);
}

/**
 * @brief Retorna o último brilho definido para o display.
 * @return Nível de brilho do display (`int8_t`).
 */  
int8_t getDisplayBrightness()
{
    return displayBrightness;
}

/// @brief Desliga o display
void turnOffDisplay()
{
    display.clear();
}