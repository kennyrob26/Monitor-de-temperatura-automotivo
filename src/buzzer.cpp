#include "buzzer.h"

#define BUZZER_PIN PB15
#define TIME_TONE 50            //Tempos em milissegundos
#define FREQUENCY 2000
#define TIME_NO_TONE 400

uint32_t previousTimeBuzzer = 0;

bool stateAlarm           = true;
bool activeBuzzer         = false;
bool previousActiveBuzzer = true;

void changeStateBuzzer();

///@brief Verifica se o motor atingiu a temperatura critica
///@return `true` se a temperatura ultrapassou o limite crítico, `false` se está dentro do esperado
bool isEngineTemperatureCritical()
{

    if(engineTemperature() > getMaxTemperatureEngine() && stateAlarm == true)
    {
        //desliga o alarme se o usuário interagir com o encoder
        if(encoder.isUpdated())
        {
            stateAlarm = false;
            setDisplayBrightness(7);
            return false;
        }

        if(activeBuzzer != previousActiveBuzzer)
        {
            previousTimeBuzzer = millis();
            previousActiveBuzzer = activeBuzzer;
        }
        changeStateBuzzer();
        return true;
    }
    else
    {
        
        if(engineTemperature() <= getMaxTemperatureEngine())
        {
            stateAlarm = true; //Ou seja quando a temperatura não for mais critica, o alarme pode disparar novamente
            activeBuzzer         = false;
            previousActiveBuzzer = true;
        }
        noTone(BUZZER_PIN);
        return false;
    }


    
}

// Altera o estado do buzzer ligado/desligado
void changeStateBuzzer()
{
    if(activeBuzzer && ((millis() - previousTimeBuzzer) < TIME_TONE))      //Ativa
        tone(BUZZER_PIN, FREQUENCY);      
    else
        activeBuzzer = false;

    if(!activeBuzzer && ((millis()) - previousTimeBuzzer) < TIME_NO_TONE)  //Desativa
        noTone(BUZZER_PIN);
    else
        activeBuzzer = true;
}
