#include <main.h>

#define IGNITION_PIN PB4   //Detecta o pós chave (carro ligado)

uint32_t startTime = 2000;

void wakeUpFromIgnition();
bool isIgnitionActive();
void unusedPins();

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

    //isEngineTemperatureCritical();
    if(!isEngineTemperatureCritical())
    {
      if(encoder.isUpdated())    
      {
          if(encoderMovedLeft())
            updateDisplayValue(getMinRecordedTemperature());
          else if(encoderMovedRight())
            updateDisplayValue(getMaxRecordedTemperature());

          delay(1000);
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
  //resetRecordedTemperatures();
  
  return false;
}


void unusedPins(){
  pinMode(PA3, OUTPUT);
  digitalWrite(PA3, LOW);

  pinMode(PA4, OUTPUT);
  digitalWrite(PA4, LOW);

  pinMode(PA8, OUTPUT);
  digitalWrite(PA8, LOW);

  pinMode(PA11, OUTPUT);
  digitalWrite(PA11, LOW);

  pinMode(PA12, OUTPUT);
  digitalWrite(PA12, LOW);

  pinMode(PA15, OUTPUT);
  digitalWrite(PA15, LOW);

  pinMode(PB1, OUTPUT);
  digitalWrite(PB1, LOW);  

  pinMode(PB3, OUTPUT);
  digitalWrite(PB3, LOW);

  pinMode(PB5, OUTPUT);
  digitalWrite(PB5, LOW);

  pinMode(PB6, OUTPUT);
  digitalWrite(PB6, LOW);

  pinMode(PB7, OUTPUT);
  digitalWrite(PB7, LOW);

  pinMode(PB8, OUTPUT);
  digitalWrite(PB8, LOW);

  pinMode(PB9, OUTPUT);
  digitalWrite(PB9, LOW);

  pinMode(PB10, OUTPUT);
  digitalWrite(PB10, LOW);

  pinMode(PB11, OUTPUT);
  digitalWrite(PB11, LOW);

  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, LOW);

  pinMode(PC14, OUTPUT);
  digitalWrite(PC14, LOW);

  pinMode(PC15, OUTPUT);
  digitalWrite(PC15, LOW);
  
}