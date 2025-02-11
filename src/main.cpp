#include <main.h>

#define IGNITION_PIN PB4   //Detecta o pós chave (carro ligado)

void wakeUpFromIgnition();
bool isIgnitionActive();

void setup() 
{
  Serial.begin(115200);
  configEEPROM();
  configDisplay();
  configMax31865();
  configEncoder();

  
  pinMode(IGNITION_PIN, INPUT_PULLDOWN);

  
  LowPower.begin();
  LowPower.attachInterruptWakeup(IGNITION_PIN, wakeUpFromIgnition, RISING, DEEP_SLEEP_MODE);
}


void loop() 
{
  if(isIgnitionActive())
  {
    showTemperature();

    if(!isEngineTemperatureCritical())
    {
      if(encoderMovesRight())
      {
        updateDisplayValue(getMaxRecordedTemperature(), 3);
        delay(2000);
      }

      checkEncoderInteraction();
    }
    
  } 

}

void wakeUpFromIgnition()
{
    configDisplay();
}

bool isIgnitionActive()
{
  if(digitalRead(IGNITION_PIN) == HIGH)
    return true;
  
  turnOffDisplay(); 
  LowPower.sleep();

  return false;
}