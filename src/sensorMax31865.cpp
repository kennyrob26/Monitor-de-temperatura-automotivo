#include "sensorMax31865.h"

#define CS_PIN  PB0
#define CLK_PIN PA5
#define SDO_PIN PA6
#define SDI_PIN PA7

#define MIN_TEMPERATURE 70

#define MAX_TEMPERATURE 150

#define REFERENCE_RESISTOR 430.0    //Valor do resistor de referência utilizado pelo MAX
#define NOMINAL_RESISTANCE 100.0    //Resistência nominal do PT100



uint8_t engineOverheatThreshold = readMaxTemperatureEEPROM();

uint8_t maxRecordedTemperature = 0;
uint8_t minRecordedTemperature = 150;

Adafruit_MAX31865 thermometer = Adafruit_MAX31865(CS_PIN, SDI_PIN, SDO_PIN, CLK_PIN);


/**
 * @brief Realiza as configurações necessárias para o funcionamento do sensor RTD através do módulo MAX31865
 * 
 * - basicamente chamamos o método begin, passando como parâmetro quantos fios possui o RTD utilizado.
 */
void configMax31865()
{
    thermometer.begin(MAX31865_3WIRE);
}

/** 
 * @brief Função responsável por coletar a temperatura atual do motor
 * 
 * - `temperature` -> recebe a temperatura coletada pelo sensor RTD, que vamos utilizar valores de 0 a 150ºC
 * 
 * - `roundf` arredonda para o inteiro mais próximo, já que valores decimais são irrelevantes neste projeto
 * 
 * - `fautsMAX31865` função que detecta se ocorreu alguma falha na leitura
 * @return retorna a temperatura atual do motor no formato uint8_t, é esperado que o valor retornado esteja entre `0 e 150ºC`
 */
uint8_t engineTemperature()
{
    uint8_t temperature = (uint8_t) roundf(thermometer.temperature(NOMINAL_RESISTANCE, REFERENCE_RESISTOR));

    updateMaxRecordedTemperature(temperature);
    updateMinRecordedTemperature(temperature);

    fautsMAX31865();                                       //Verifica se ocorreram falhas na leitura

    return(temperature);
}

//Função que verifica se falhas ocorreram na leitura do sensor, segue o padrão de exemplo da biblioteca
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

/**
 * @brief Define a `temperatura maxima que o motor pode atingir`, é um ponto crucial de todo o código pois define a temperatura critica. Com base na temperatura critica nós conseguimos `alertar o usuário` caso a temperatura do motor seja maior que a temperatura critica.
 * 
 * @param maxTemperature recebe um uint8_t, o valor recebido será definido como a temperatura critica do sistema, é esperado valores entre 0 e 150ºC, mas em definições reais a temperatura critica deverá ficar entre 90 e 100ºC
 * 
 */
void setEngineOverheatThreshold(u_int8_t maxTemperature)
{
  engineOverheatThreshold = maxTemperature;
}

/**
 * @brief Função responsável por retornar a temperatura critica do sistema.
 * @return A função retorna o conteúdo de `maxTemperatureEngine`, variável que armazena a temperatura critica do sistema. O valor retornado é do tipo `uint8_t`que retorna valores entre 0 a 150ºC
 */
uint8_t getMaxTemperatureEngine()
{
    return engineOverheatThreshold;
}

/**
 * @brief Atualiza a temperatura máxima coletada pelo sistema, ou seja qual foi o pico, ou a maior temperatura coletada.
 * @param temperatureNow é a temperatura atual, sempre que o sistema obtém a temperatura atual, esta será comparada com `maxRecordedTemperature`, se a temperatura atual for maior do que a maior registrada, então a temperatura atual passa a ser a nova temperatura máxima
 */
void updateMaxRecordedTemperature(uint8_t temperatureNow)
{
  if (maxRecordedTemperature < temperatureNow)
    maxRecordedTemperature = temperatureNow;
}

/**
 * @brief Retorna a temperatura máxima que o sistema atingiu durante o funcionamento. Este valor é resetado sempre que o veículo desliga.
 * @return retorna maxRecordedTemperature, que nada mais é do que a temperatura máxima registrada durante o funcionamento do veículo.
 */
uint8_t getMaxRecordedTemperature()
{
  return maxRecordedTemperature;
}

/**
 * @brief Atualiza a temperatura mínima coletada pelo sistema, ou seja qual foi a menor temperatura coletada.
 * @param temperatureNow é a temperatura atual, sempre que o sistema obtém a temperatura atual, esta será comparada com `minRecordedTemperature`, se a temperatura atual for menor do que a menor registrada, então a temperatura atual passa a ser a nova temperatura mínima
 */
void updateMinRecordedTemperature(uint8_t temperatureNow)
{
  if(minRecordedTemperature > temperatureNow)
    minRecordedTemperature = temperatureNow;
}

/**
 * @brief Retorna a temperatura mínima que o sistema atingiu durante o funcionamento. Este valor é resetado sempre que o veículo desliga.
 * @return retorna minRecordedTemperature, que nada mais é do que a temperatura mínima registrada durante o funcionamento do veículo.
 */
uint8_t getMinRecordedTemperature()
{
  return minRecordedTemperature;
  //return minRecordedTemperature;
}

//// @brief Reseta os valores mínimos e máximos de temperatura definido
void resetRecordedTemperatures()
{
  maxRecordedTemperature = MIN_TEMPERATURE;
  minRecordedTemperature = MAX_TEMPERATURE;
}