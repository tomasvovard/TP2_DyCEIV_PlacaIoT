#include "tank_io.h"

void io_update_inputs(inputs_t *inputs){
  /*Lee el registro del spi y actualzia inputs*/
  uint8_t in = spi_transf_digitalInput(vspi);

  //botones
  inputs->btn_auto   = (in & IO_BTN_AUTO_MASK) != 0;
  inputs->btn_b1     = (in & IO_BTN_B1_MASK)   != 0;
  inputs->btn_b2     = (in & IO_BTN_B2_MASK)   != 0;
  inputs->btn_parada = (in & IO_BTN_PRD_MASK)  != 0;

  //sensores
  inputs->sn_bajo    = (in & IO_SN_BAJO_MASK)  != 0;
  inputs->sn_medio   = (in & IO_SN_MED_MASK)   != 0;
  inputs->sn_alto    = (in & IO_SN_ALTO_MASK)  != 0;
}

void io_update_output(outputs_t *outputs){
  /*Construye el byte de salida y actualiza por spi*/
  uint8_t out = 0;
  
  if(outputs->motor1) out |= IO_MOTOR1_MASK;
  if(outputs->motor2) out |= IO_MOTOR2_MASK;

  spi_transf_digitalOutput(vspi, out);
}

