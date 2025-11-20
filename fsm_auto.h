/*MEF de control automático de las bombas.
  AUTO_HIGH: el tanque está con nivel alto, bombas apagadas.
  AUTO_ONE_PUMP: el nivel cayó por debajo de alto y se enciende una de las bombas.
  AUTO_REST: período de descanso de bombas tras llenar el tanque.
  AUTO_TWO_PUMP: el nivel cayó por debajo de medio por más de 5 minutos y se encendieron ambas bombas.

  La mef utiliza temporizadores en ticks de la tarea lógica del RTOS (TASK_LG_PERIOD_MS) para implementar la temporización de descanso y de permanencia de bajo nivel.
*/
#pragma once

#include "tank_io.h"

#define TASK_LG_PERIOD_MS 100

// 5 minutos expresados en "ticks" de la tarea de lógica
#define AUTO_5MIN_MS        (10UL*1000UL)//(5UL*60UL*1000UL)
#define AUTO_5MIN_TICKS     (AUTO_5MIN_MS / TASK_LG_PERIOD_MS)

typedef enum {AUTO_HIGH, AUTO_ONE_PUMP, AUTO_REST, AUTO_TWO_PUMP} fsm_auto_t;

void fsm_auto_init();
void fsm_auto(const inputs_t *inputs, outputs_t *outputs);
