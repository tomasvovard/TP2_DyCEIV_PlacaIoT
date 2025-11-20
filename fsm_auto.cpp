#include "fsm_auto.h"

/*variable de estado de la mef*/
static fsm_auto_t state_auto;

/*Estado interno de las IO*/
static bool auto_p1_on = false;
static bool auto_p2_on = false;

static bool sn_high = false;
static bool sn_mid = false;
static bool sn_low = false;

/*variable para conmutar entre una bomba y otra*/
static bool last_pump = false;

/*contadores de los timers*/
static uint32_t rest_ticks = 0;
static uint32_t below_mid_ticks = 0;

/*timer de descanso*/
static void rest_timer(){
  if(rest_ticks > 0) rest_ticks--; 
}

/*timer de nivel bajo*/
static void below_mid_timer(){
  if(below_mid_ticks > 0) below_mid_ticks--; 
}

void fsm_auto_init(){
  state_auto = AUTO_HIGH;
  auto_p1_on = false;
  auto_p2_on = false;
  rest_ticks = 0;
  below_mid_ticks = AUTO_5MIN_TICKS;
}

/*mef*/
void fsm_auto(const inputs_t *inputs, outputs_t *outputs){

  sn_high = inputs->sn_alto;
  sn_mid = inputs->sn_medio;
  sn_low = inputs->sn_bajo;

  /*Temporización nivel bajo (independiente de los estados)
    se implementa una cuenta regresiva mientras el nivel esté por debajo de medio
  */
  if(!sn_mid){
    below_mid_timer();
  }
  else
    below_mid_ticks = AUTO_5MIN_TICKS;
  
  switch(state_auto){

    case AUTO_HIGH:

      auto_p1_on = false;
      auto_p2_on = false;

      if(!sn_high){
        state_auto = AUTO_ONE_PUMP;
        auto_p1_on = !last_pump;
        auto_p2_on = last_pump;
      }
      break;

    case AUTO_ONE_PUMP:

      if(below_mid_ticks <= 0){
        state_auto = AUTO_TWO_PUMP;
        auto_p1_on = true;
        auto_p2_on = true;
      }

      if(sn_high){
        state_auto = AUTO_REST;
        auto_p1_on = false;
        auto_p2_on = false;
        rest_ticks = AUTO_5MIN_TICKS;
        last_pump = !last_pump;
      }
      break;

    case AUTO_REST:
      
      rest_timer();

      /*Al terminar el período de descanso puede ocurrir que le nivel no esté en alto. En ese caso se evalía si hay que encender una o ambas bombas.*/
      if(rest_ticks <= 0){
        if(!sn_high){
          if(below_mid_ticks <= 0){
            state_auto = AUTO_TWO_PUMP;
            auto_p1_on = true;
            auto_p2_on = true;
          }
          else{
            state_auto = AUTO_ONE_PUMP;
            auto_p1_on = !last_pump;
            auto_p2_on = last_pump;
          }
        }
        else
          state_auto = AUTO_HIGH;
      }
      break;

    case AUTO_TWO_PUMP:
    
      if(sn_high){
        state_auto = AUTO_REST;
        rest_ticks = AUTO_5MIN_TICKS;
        auto_p1_on = false;
        auto_p2_on = false;
        last_pump = !last_pump;
      }
      break;
  }

  outputs->motor1 = auto_p1_on;
  outputs->motor2 = auto_p2_on;
}