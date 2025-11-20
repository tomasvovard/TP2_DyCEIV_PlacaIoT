/*MEF de control de modos de operación.
  MODE_OFF: sistema apagado.
  MODE_MANUAL: cada botón acciona la correspondiente bomba.
  MODE_AUTO: control de nivel automático con lógica de fsm_auto.
*/
#pragma once

#include "tank_io.h"
#include "fsm_auto.h"

typedef enum {MODE_OFF, MODE_MANUAL, MODE_AUTO} fsm_mode_t;

void fsm_mode_init();
void fsm_mode(const inputs_t *inputs, outputs_t *outputs);