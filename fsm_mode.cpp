#include "fsm_mode.h"

/*variable de debug*/
static bool update = true;

/*variable de estado de la mef*/
static fsm_mode_t state_mode;

void fsm_mode_init(){
  state_mode = MODE_OFF;
}

void fsm_mode(const inputs_t *inputs, outputs_t *outputs){
  switch(state_mode)
  {
    case MODE_OFF:

      outputs->motor2 = false;
      outputs->motor1 = false;

      if(inputs->btn_b1 || inputs->btn_b2){
        state_mode = MODE_MANUAL;
        Serial.println("mefModo: MODE_MANUAL");
      }

      if(inputs->btn_auto){
        state_mode = MODE_AUTO;
        Serial.println("mefModo: MODE_AUTO");
        fsm_auto_init();
      }

      break;

    case MODE_MANUAL:

      outputs->motor1 = inputs->btn_b1;
      outputs->motor2 = inputs->btn_b2;

      if(inputs->btn_parada){
        state_mode = MODE_OFF;
        Serial.println("mefModo: MODE_OFF");
      }

      if(inputs->btn_auto){
        state_mode = MODE_AUTO;
        Serial.println("mefModo: MODE_AUTO");
        fsm_auto_init();
      }

      break;
    
    case MODE_AUTO:

      fsm_auto(inputs, outputs);

      if(inputs->btn_b1 || inputs->btn_b2){
        state_mode = MODE_MANUAL;
        Serial.println("mefModo: MODE_MANUAL");
      }

      if(inputs->btn_parada){
        state_mode = MODE_OFF;
        Serial.println("mefModo: MODE_OFF");
      }

      break;
  }

  if(update){
    Serial.print("mefModo | Btn Auto: ");
    Serial.print(inputs->btn_auto);
    Serial.print("; Btn B1: ");
    Serial.print(inputs->btn_b1);
    Serial.print("; Btn B2: ");
    Serial.print(inputs->btn_b2);
    Serial.print("; Btn off: ");
    Serial.print(inputs->btn_parada);
    Serial.print("; Motor 1: ");
    Serial.print(outputs->motor1);
    Serial.print("; Motor 2: ");
    Serial.print(outputs->motor2);
    Serial.println();
    update = false;
  }
}