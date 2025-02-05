#include "sensorMax31865.h"

#define CS_PIN  PB0
#define CLK_PIN PA5
#define SDO_PIN PA6
#define SDI_PIN PA7

#define REFERENCE_RESISTOR 430.0    //Valor do resistor de referência utilizado pelo MAX
#define NOMINAL_RESISTANCE 100.0    //Resistência nominal do PT100

Adafruit_MAX31865 thermometer = Adafruit_MAX31865(CS_PIN, SDI_PIN, SDO_PIN, CLK_PIN);

void configMax31865()
{
    thermometer.begin(MAX31865_3WIRE);
}

uint8_t engineTemperature()
{
    uint8_t temperature = thermometer.temperature(NOMINAL_RESISTANCE, REFERENCE_RESISTOR);

    temperature = roundf(temperature);                     //Arredonda para o inteiro mais próximo

    fautsMAX31865();                                       //Verifica se ocorreram falhas na leitura

    return(temperature);
}

//Verifica se existem falhas na leitura
void fautsMAX31865()
{
      // Check and print any faults
  uint8_t fault = thermometer.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    thermometer.clearFault();
  }
}

