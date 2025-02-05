#include "buzzer.h"

#define BUZZER_PIN PB15
#define TIME_TONE 50
#define FREQUENCY 2200
#define TIME_NO_TONE 400

uint32_t previousTimeBuzzer = 0;

bool activeBuzzer = false;
bool previousActiveBuzzer = true;

void changeStateAlarm()
{

    if(activeBuzzer && ((millis() - previousTimeBuzzer) < TIME_TONE))
    {
        Serial.println(1);
        tone(BUZZER_PIN, FREQUENCY);
        
    }
    else
        activeBuzzer = false;

    if(!activeBuzzer && ((millis()) - previousTimeBuzzer) < TIME_NO_TONE)
    {
        Serial.println(0);
        noTone(BUZZER_PIN);
    }
    else
        activeBuzzer = true;
}


void needActivateAlarm()
{
    if(engineTemperature() > getMaxSetTemperature())
    {
        Serial.println(1);
        if(activeBuzzer != previousActiveBuzzer)
        {
            previousTimeBuzzer = millis();
            previousActiveBuzzer = activeBuzzer;
        }
        changeStateAlarm();
    }
    
}