#include "fsm_mode.h"

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

      if(inputs->btn_b1 || inputs->btn_b2)
        state_mode = MODE_MANUAL;

      if(inputs->btn_auto){
        state_mode = MODE_AUTO;
        fsm_auto_init();
      }

      break;

    case MODE_MANUAL:

      outputs->motor1 = inputs->btn_b1;
      outputs->motor2 = inputs->btn_b2;

      if(inputs->btn_parada)
        state_mode = MODE_OFF;

      if(inputs->btn_auto){
        state_mode = MODE_AUTO;
        fsm_auto_init();
      }

      break;
    
    case MODE_AUTO:

      fsm_auto(inputs, outputs);

      if(inputs->btn_b1 || inputs->btn_b2) state_mode = MODE_MANUAL;

      if(inputs->btn_parada)
        state_mode = MODE_OFF;

      break;
  }
}