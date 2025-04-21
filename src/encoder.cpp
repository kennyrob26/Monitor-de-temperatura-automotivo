#include "encoder.h"

#define BTN_ENCODER   PA2     //Botão do encoder

STM32encoder encoder(TIM2);
bool isFirstEncoderInteraction = false;

volatile uint32_t lastButtonPressTime  =     0;
volatile uint8_t  buttonPressDuration  =     0;  //em segundos
volatile bool     isConfigMode     = false; 


/**
 * @brief Configuração inicial do encoder
 * 
 * Define qual pino será utilizado pelo botão do encoder, e sua respectiva interrupção
 */
void configEncoder()
{
    pinMode(BTN_ENCODER, INPUT);
    attachInterrupt(digitalPinToInterrupt(BTN_ENCODER), measureButtonPressTime, CHANGE);
}

/**
 * @brief Função Callback chamada pela interrupção gerada BTN_ENCODER, tem como objetivo calcular quanto tempo o botão ficou pressionado  
 * 
 * - Na borda de descida (LOW) reseta o tempo atribuido a variável `lastButtonPressTime`
 * 
 * - Na borda de subida (HIGH) calcula o tempo em que ficou pressionado com base na diferença entre `micros()` e `lastButtonPressTime`
 * 
 * - `buttonPressDuration` recebe o tempo pressionamento em segundos, arredondado para o inteiro mais próximo.
 * 
 * - `isConfigMode` flag que é `true` se existir interação com o botão, permitindo tratar fora da interrupção a interação com o encoder 
 */
void measureButtonPressTime()
{
  if(digitalRead(BTN_ENCODER) == LOW)
    lastButtonPressTime = micros();
  else
  {
    uint32_t pressedDurationMicros = micros() - lastButtonPressTime;
    buttonPressDuration = (pressedDurationMicros + 500000) / 1000000;
    isConfigMode = true;
  }   
}

/**
 * @brief Função que trata as interações com o encoder, ela é chamada sempre que `measureButtonPressTime()` define a flag`isConfigMode` como `true`
 * 
 * O `switch` verifica o valor de `buttonPressedDuration`, e com base no tempo pressionado o encoder realiza ações diferentes
 * 
 * - `0 segundo` (click) -> conseguimos alterar o brilho do display rotacionando o encoder (-1 a 7)
 * 
 * - `2 segundos` ->  conseguimos definir a temperatura critica do motor rotacionando o encoder (0 a 150ºC), após definida a temperatura ela é salva na EEPROM utilizando a função `writeMaxTemperatureEEPROM`
 * 
 * - `4 segundos` -> chama a função `resetEEPROM()`resetando a EEPROM para os valores default
 * 
 * Se o usuário deixar o encoder inativo por 1.5s o sistema sai do modo de configuração
 */
void checkEncoderInteraction()
{
    uint16_t encoderInactiveTime         = 0;    //Tempo de inatividade do encoder
    uint32_t lastEncoderInteractionTime  = 0;    //Tempo do ultimo acionamento do encoder

    if(isConfigMode)
    {
        isFirstEncoderInteraction  = true;
        lastEncoderInteractionTime = millis();

        switch (buttonPressDuration)
        {
            //Click
            case 0:
                if(getDisplayBrightness() < 0)
                {
                    setDisplayBrightness(7);
                }
                else
                {
                    do
                    {     
                        if(encoder.isUpdated() || isFirstEncoderInteraction )
                        {
                            lastEncoderInteractionTime = millis();
                            encoderSetsDisplayBrightness();
                        }
                        encoderInactiveTime = millis() - lastEncoderInteractionTime;
                        isFirstEncoderInteraction  = false;
                    } while (encoderInactiveTime <= 1500);
                }
                break;

            //Pressionado por 2 segundos
            case 1:
            case 2:
                do
                {     
                    if(encoder.isUpdated() || isFirstEncoderInteraction )
                    {
                        lastEncoderInteractionTime = millis();
                        encoderSetsTemperatureEngine();
                    }
                    encoderInactiveTime = millis() - lastEncoderInteractionTime;
                    isFirstEncoderInteraction  = false;
                } while (encoderInactiveTime <= 1500);
                writeMaxTemperatureEEPROM(getMaxTemperatureEngine());
                break;
            
            case 4:
                resetEEPROM();
                break;

            default:
                break;
        }
        isConfigMode      = false;
    }
}

/**
 * @brief Função chamada quando o `BTN_ENCODER` é clicado. Define o brilho do display através da rotação do encoder
 * 
 * - `brightness` recebe o brilho atual do display
 * 
 * - `encoder.bind` incrementa ou decrementa 1 de `brightness` com base na rotação do encoder, os valores válidos vão de `-1 a 7`, onde `-1 desliga` o display, e `7 é o brilho máximo`.
 * 
 * - `setDisplayBrightness` aplica o brilho no display, e `updateDisplayValue` demonstra o nível atual do brilho
 */
void encoderSetsDisplayBrightness()
{   
    static int8_t brightness;

    if(brightness == -1 && getDisplayBrightness() == 7)
        brightness = getDisplayBrightness();

    encoder.bind(&brightness, 1, -1, 7);
    
    setDisplayBrightness(brightness);

    updateDisplayValue(brightness);
    
    
}

/**  
 * @brief Função chamada quando o `BTN_ENCODER` é pressionado por 2 segundos. Altera a temperatura critica do motor com base na rotação do encoder
 * 
 * - `maxTemperature` recebe a ultima temperatura critica setada
 * 
 * - `encoder.bind` incrementa ou decrementa em 1 `maxTemperature`com base na rotação do encoder, os valores válidos são de `0 à 150ºC`
 * 
 * - `setMaxTemperature` aplica a nova temperatura definida pelo encoder, enquanto `updateDisplayValue` demonstra o novo valor
 */
void encoderSetsTemperatureEngine()
{
    static uint8_t maxTemperature = getMaxTemperatureEngine();
    encoder.bind(&maxTemperature, 1, 0, 150);
    setEngineOverheatThreshold(maxTemperature);
    updateDisplayValue(maxTemperature);
}

/**
 * @brief Retorna se o encoder foi movido para a direita na ultima atualização, permitindo tomar decisões a partir desta informação
 */
bool encoderMovedRight()
{
    if(encoder.dir() == 1)
        return true;

    return false;
}

/**
 * @brief Retorna se o encoder foi movido para a esquerda na ultima atualização, permitindo tomar decisões a partir desta informação
 */
bool encoderMovedLeft()
{
    if(encoder.dir() == 0)
        return true;

    return false;
}

