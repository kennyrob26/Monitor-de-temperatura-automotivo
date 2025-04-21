#include "display.h"

#define CLK_PIN PB13
#define DIO_PIN PB12

uint64_t lastDisplayUpdate = 0;

TM1637Display display(CLK_PIN, DIO_PIN);

int8_t displayBrightness  = 7;
uint8_t buffer_brightness = 7;
bool displayState = 1;

uint8_t lastDisplayValue = 255;

/// @brief Configurações iniciais do display
void configDisplay()
{
    display.setBrightness(displayBrightness);
    lastDisplayValue = 255;
}

/**
 * @brief Atualiza o valor exibido pelo display, sempre verificamos se o novo valor é igual ao ultimo valor exibido, em caso afirmativo não é necessário atualizar o display
 * @param value valor que será exibido pelo display (0 a 255)
 * @param length quantidade de dígitos
 */
void updateDisplayValue(int8_t value)
{
    if(lastDisplayValue != value)
    {
        lastDisplayValue = value;
        display.showNumberDec(value);
    }
}

/// @brief Exibe a temperatura atual no display, se o sistema entrar em modo crítico, ou seja, o motor ultrtapassar a temperatura definida, o diplay começa a piscar demonstrando a temperatura atual, com isso conseguimos desviar a atenção do usuário para o display
void showTemperature()
{   
    uint8_t temperature = engineTemperature();

    //Entra em modo critico (pisca display)
    if(isEngineTemperatureCritical())
    {
        if((millis() - lastDisplayUpdate ) > 250)
        {
            lastDisplayValue = 0;               //Força a atualização do display
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
    else
    {
        //Garante que o display voltará ligado com o ultimo brilho setado após o alarme
        if(displayBrightness != buffer_brightness)
            displayBrightness = buffer_brightness;
    }

    updateDisplayValue(temperature);
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
    {
        display.setBrightness(displayBrightness);

        if(!isEngineTemperatureCritical())
            buffer_brightness = displayBrightness;         //Salva o ultimo brilho diferente de -1
    }
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