/*
  Módulo de vinculación de IOs físicas con conceptos.
  Mapea bits del registro del SPI con señales lógicas.
*/
#pragma once

#include "spi_hardware.h"

#define TAKS_IO_PERIOD_MS 50

#define IO_BTN_AUTO_MASK  0b00000001
#define IO_BTN_B1_MASK    0b00000010
#define IO_BTN_B2_MASK    0b00000100
#define IO_BTN_PRD_MASK   0b00001000
#define IO_SN_BAJO_MASK   0b00010000
#define IO_SN_MED_MASK    0b00100000
#define IO_SN_ALTO_MASK   0b01000000

#define IO_MOTOR1_MASK 0b00000001
#define IO_MOTOR2_MASK 0b00000010

typedef struct{
  bool btn_auto;
  bool btn_b1;
  bool btn_b2;
  bool btn_parada;
  bool sn_bajo;
  bool sn_medio;
  bool sn_alto;
} inputs_t;

typedef struct{
  bool motor1;
  bool motor2;
} outputs_t;

void io_update_inputs(inputs_t *inputs);
void io_update_output(outputs_t *outputs);