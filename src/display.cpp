#include "display.h"

#define CLK_PIN PB13
#define DIO_PIN PB12

TM1637Display display(CLK_PIN, DIO_PIN);


uint8_t maxSetTemperature = 90;


STM32encoder encoder(TIM2);
bool firstOccurrence = false;

int8_t brightness  = 7;


void configDisplay()
{
    display.setBrightness(brightness);
    //display.showNumberDec(100, false);

}
void showTemperature()
{
    
    uint8_t temperature = engineTemperature();
    display.showNumberDec(temperature, false, 3, 1);
    //display.showNumberHexEx(0xc, 0, false, 1, 3);
    

}
void setBrightnessDisplay()
{   
    encoder.bind(&brightness, 1, -1, 7);

    display.showNumberDec(brightness );

    if(brightness >= 0)
        display.setBrightness(brightness);
    else
        display.setBrightness(7, false);

}

void setMaxTemperature()
{
    encoder.bind(&maxSetTemperature, 1, 0, 120);
    display.showNumberDec(maxSetTemperature);
}

uint8_t getMaxSetTemperature()
{
    return maxSetTemperature;
}

void turnOffDisplay()
{
    display.clear();
}