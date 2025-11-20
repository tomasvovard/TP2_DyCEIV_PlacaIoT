/*Test de funcionamiento del SPI. Conmuta todas las salidas una por una, lee el estado de las entradas y muestra resultados por puerto serie*/
#pragma once

#include "soc/gpio_reg.h"
#include <Arduino.h>

void test_printHex8(uint8_t val);
void test_printBin8(uint8_t val);
void test_digitalIO(uint8_t outputs);
void test_sequenceDigitalIO();
