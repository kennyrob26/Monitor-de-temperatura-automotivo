#include <main.h>

#define INGNITION_PIN PB4   //Detecta o pós chave (carro ligado)
#define BTN_ENCODER   PA2     //Botão do encoder

volatile uint32_t previousTime   =     0;
volatile uint8_t  pressedTimeBTN =     0;
volatile bool     configMode     = false; 

void btnCallBack();
void ignitionCallBack();
bool ignitionIsActive();

void setup() 
{
  Serial.begin(115200);
  configDisplay();
  configMax31865();

  pinMode(BTN_ENCODER, INPUT);
  pinMode(INGNITION_PIN, INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(BTN_ENCODER), btnCallBack, CHANGE);
  LowPower.begin();
  LowPower.attachInterruptWakeup(INGNITION_PIN, ignitionCallBack, RISING, DEEP_SLEEP_MODE);
  //attachInterrupt(digitalPinToInterrupt(INGNITION_PIN), ignitionCallBack, CHANGE);
}


void loop() 
{
  if(ignitionIsActive())
  {
    needActivateAlarm();
    showTemperature();

    uint16_t loopTime            = 0;    //Verifica se o tempo já esgotou
    uint32_t previousTimeEncoder = 0;    //Acrescenta mais tempo se houver interação
    if(configMode)
    {
      Serial.println(pressedTimeBTN);
      firstOccurrence = true;

      switch (pressedTimeBTN)
      {
      case 0:
        if(brightness < 0)
          brightness = 7;
          
        previousTimeEncoder = millis();
        do
        {     
          if(encoder.isUpdated() || firstOccurrence)
          {
            previousTimeEncoder = millis();
            setBrightnessDisplay();
          }
          loopTime = millis() - previousTimeEncoder;
          firstOccurrence = false;
        } while (loopTime <= 1500);
        Serial.println("click");

        break;

      case 1:
      case 2:
        previousTimeEncoder = millis();
        do
        {     
          if(encoder.isUpdated() || firstOccurrence)
          {
            previousTimeEncoder = millis();
            setMaxTemperature();
          }
          loopTime = millis() - previousTimeEncoder;
          firstOccurrence = false;
        } while (loopTime <= 1500);
        Serial.println("1 seconds pressed!");
        break;

      case 5:
        Serial.println("5 seconds pressed");
      default:
        break;
      }

      
      configMode      = false;
    }
  }

  //Serial.println(engineTemperature());
  //setTemperature();

}


void btnCallBack()
{
  if(digitalRead(BTN_ENCODER) == LOW)
    previousTime = micros();
  else
  {
    uint32_t pressedTimeMicros = micros() - previousTime;
    pressedTimeBTN = roundf(pressedTimeMicros / 1000000.0);
    configMode = true;
  }   
}


void ignitionCallBack()
{
    configDisplay();
}

bool ignitionIsActive()
{
  if(digitalRead(INGNITION_PIN) == HIGH)
    return true;
  
  turnOffDisplay(); 

  detachInterrupt(digitalPinToInterrupt(BTN_ENCODER));
  LowPower.sleep();
  attachInterrupt(digitalPinToInterrupt(BTN_ENCODER), btnCallBack, CHANGE);

  return false;
}